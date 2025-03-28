#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/Orc/LLJIT.h>
#include <llvm/ExecutionEngine/Orc/ExecutionUtils.h>
#include <llvm/Support/Error.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/ExecutionEngine/Orc/ThreadSafeModule.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/CodeGen/CommandFlags.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/TargetParser/Host.h>
#include "llvm/Support/VirtualFileSystem.h"

#include "backend/generator/generate_ir.hpp"
#include "backend/generator/symbols/symbol_stack.hpp"

extern "C" {
    #include "utils.h"
    #include "args/definitions.h"
    #include "frontend/lexer/core.h"
    #include "frontend/lexer/freeTokens.h"
    #include "frontend/ast/definitions.h"
    #include "frontend/parser/core.h"
    #include "frontend/parser/printer/print_ast.h"
    #include "frontend/getTokenTypeName.h"
    #include "frontend/freeTokens.h"
}

#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include <fstream>
#include <filesystem> 
#include <unordered_set>
#include <sstream>


int startREPL() {
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    auto JIT = llvm::orc::LLJITBuilder().create();
    if (!JIT) {
        llvm::errs() << "Erro ao inicializar LLJIT: " << llvm::toString(JIT.takeError()) << "\n";
        return 1;
    }

    llvm::LLVMContext TheContext;

    enter_scope();

    std::cout << "(GalaxyJIT REPL v1.0.0) Enter code below ('exit' to quit):\n";

    std::string Line;
    std::vector<std::string> codeLines;

    while (true) {
        std::cout << ">>> ";
        std::getline(std::cin, Line);
        if (Line.empty()) continue;
        if (Line == "exit") break;

        codeLines.push_back(Line);
            
        auto TheModule = std::make_unique<llvm::Module>("GalaxyJIT", TheContext);
        llvm::IRBuilder<llvm::NoFolder> Builder(TheContext);

        auto MainTracker = (*JIT)->getMainJITDylib().getDefaultResourceTracker();
        if (auto Err = MainTracker->remove()) {
            llvm::errs() << "Erro ao remover o módulo anterior do JIT: " << llvm::toString(std::move(Err)) << "\n";
            continue;
        }

        std::vector<std::string> modules {
            "libs/std.ll"
        };

        for (const auto &file : modules) {
            llvm::SMDiagnostic Err;

            // Carregar o módulo
            auto Buffer = llvm::MemoryBuffer::getFile(file);
            if (!Buffer) {
                std::cout << "Error loading std.ll";
                continue;
            }

            auto Module = llvm::parseIR(**Buffer, Err, TheContext);
            if (!Module) {
                llvm::errs() << "Erro ao analisar IR: " << Err.getMessage() << "\n";
                continue;
            }


            // Adicionar o módulo ao JIT
            if (auto Err = (*JIT)->addIRModule(llvm::orc::ThreadSafeModule(std::move(Module), std::make_unique<llvm::LLVMContext>()))) {
                llvm::errs() << "Erro ao adicionar o módulo " << file << " ao JIT: " << llvm::toString(std::move(Err)) << "\n";
                continue; // Pula para o próximo módulo
            }
        }

        std::ofstream stempFile(".repl.glx", std::ios::out);
        if (!stempFile) {
            std::cerr << "Erro ao abrir arquivo temporário.\n";
            return 1;
        }

        // Escrever as linhas de código no arquivo temporário
        for (const auto& line : codeLines) {
            stempFile << line << "\n"; // Usando operador << para escrever a linha
        }

        stempFile.close();

        FILE *tempFile = fopen(".repl.glx", "r");
        int count = 0;
        Token *tokens = tokenize(tempFile, ".repl.glx", &count);
        fclose(tempFile);

        Parser parser = parser_new();
        AstNode *ast = produce_ast(&parser, tokens, count, true);
        if (!ast) {
            std::cerr << "Erro: Entrada inválida ou falha ao analisar AST.\n";
            freeTokens(tokens, count);
            continue;
        }

        // Criar a função entry.main
        llvm::FunctionType *mainFuncType = llvm::FunctionType::get(Builder.getVoidTy(), false);
        llvm::Function *mainFunc = llvm::Function::Create(mainFuncType, llvm::GlobalValue::WeakODRLinkage, "entry.main", *TheModule);

        llvm::BasicBlock *entryBlock = llvm::BasicBlock::Create(TheContext, "entry", mainFunc);
        Builder.SetInsertPoint(entryBlock);

        generate_ir(ast, TheContext, *TheModule, Builder);

        Builder.CreateRetVoid();

        ProgramNode *programNode = (ProgramNode *)ast->data;

        for (int i = 0; i < programNode->statement_count; i++) {
            // Acessando diretamente a string em codeLines[i]
            auto& currentCodeLine = codeLines[i];

            // Verifica se o statement está dentro dos casos desejados
            switch (programNode->statements[i]->kind) {
                case NODE_EXTERN:
                case NODE_FUNCTION:
                case NODE_VARIABLE:
                case NODE_ASSIGNMENT: {
                    // Verifica se 'currentCodeLine' é igual a 'Line' e, em caso afirmativo, move para o final
                    if (currentCodeLine == Line) {
                        // Remove a linha de onde está (atualmente 'i')
                        codeLines.erase(codeLines.begin() + i);
                        // Adiciona a linha no final do vetor
                        codeLines.push_back(Line);
                        continue;  // Encontramos e movemos a linha, não precisamos continuar
                    }
                }
                break; // Termina o case

                case NODE_CALL:
                case NODE_WHILE:
                case NODE_FOR: {
                    // Para os outros tipos de statement, apenas remove a linha
                    if (currentCodeLine == Line) {
                        // Remove a linha de onde está (atualmente 'i')
                        codeLines.erase(codeLines.begin() + i);
                        // Adiciona a linha no final do vetor
                        continue;  // Encontramos e movemos a linha, não precisamos continuar
                    }
                }
                break; // Termina o case
            }
        }

        // Verificar o módulo
        std::string errorMsg;
        llvm::raw_string_ostream errorStream(errorMsg);
        if (llvm::verifyModule(*TheModule, &errorStream)) {
            llvm::errs() << "Erro ao verificar módulo:\n" << errorStream.str() << "\n";
            free_ast_node(ast);
            freeTokens(tokens, count);
            continue;
        }

        // Emitir o módulo em formato texto para o arquivo "repl.ll"
        std::error_code EC;
        llvm::SMDiagnostic Err;

        std::string file = ".repl.ll";

        llvm::raw_fd_ostream outFile(file, EC, llvm::sys::fs::OF_Text);
        if (EC) {
            llvm::errs() << "Erro ao abrir arquivo de saída para escrita: " << EC.message() << "\n";
            free_ast_node(ast);
            freeTokens(tokens, count);
            continue;
        }

        // Escrever o módulo no arquivo
        TheModule->print(outFile, nullptr);

        // Adicionar o módulo ao JIT (sem usar o ResourceTracker diretamente)
        auto Module = llvm::parseIRFile(file, Err, TheContext);
        if (!Module) {
            llvm::errs() << "Erro ao carregar o arquivo " << file << ": " << Err.getMessage() << "\n";
            continue; // Pula para o próximo módulo
        }

        // Adicionar o módulo ao JIT
        if (auto Err = (*JIT)->addIRModule(llvm::orc::ThreadSafeModule(std::move(Module), std::make_unique<llvm::LLVMContext>()))) {
            llvm::errs() << "Erro ao adicionar o módulo " << file << " ao JIT: " << llvm::toString(std::move(Err)) << "\n";
            continue; // Pula para o próximo módulo
        }

        auto Func = (*JIT)->lookup("entry.main");
        if (!Func) {
            llvm::errs() << "Error: entry.main not found.\n";
            free_ast_node(ast);
            freeTokens(tokens, count);
            continue;
        }

        auto EntryMain = (void (*)())(Func->getValue());
        EntryMain();

        free_ast_node(ast);
        freeTokens(tokens, count);
    }

    return 0;
}

bool generate_output(llvm::Module &TheModule, const std::string &output_type, const std::string &output_file) {
    if (output_type == "ir") {
        // Gerar LLVM IR
        std::error_code EC;
        llvm::raw_fd_ostream outFile(output_file, EC, llvm::sys::fs::OF_Text);
        if (EC) {
            llvm::errs() << "Erro ao abrir o arquivo de saída: " << EC.message() << "\n";
            return false;
        }
        TheModule.print(outFile, nullptr);
        return true;
    } else if (output_type == "object") {
        // Gerar objeto ou binário
        llvm::InitializeAllTargetInfos();
        llvm::InitializeAllTargets();
        llvm::InitializeAllTargetMCs();
        llvm::InitializeAllAsmParsers();
        llvm::InitializeAllAsmPrinters();

        auto TargetTriple = llvm::sys::getDefaultTargetTriple();
        TheModule.setTargetTriple(TargetTriple);
    
        std::string Error;
        auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);
    
        if (!Target) {
            llvm::errs() << Error;
            return 1;
        }
    
        // Specify the target CPU and features (e.g., generic)
        auto CPU = "generic";
        auto Features = "";
    
        // Set the target options and create the target machine
        llvm::TargetOptions opt;
        auto TheTargetMachine = Target->createTargetMachine(
            TargetTriple, CPU, Features, opt, llvm::Reloc::PIC_);
    
        // Set the data layout for the module based on the target machine
        TheModule.setDataLayout(TheTargetMachine->createDataLayout());
    
        // Write the generated IR to a file
        std::error_code EC;
        auto ObjectFile = output_file;
        llvm::raw_fd_ostream ObjFile(ObjectFile, EC, llvm::sys::fs::OF_None);
    
        if (EC) {
            llvm::errs() << "Could not open file: " << EC.message();
            return 1;
        }
    
        llvm::legacy::PassManager pass;
        auto FileType = llvm::CodeGenFileType::ObjectFile;

        // Add passes to generate the object file
        if (TheTargetMachine->addPassesToEmitFile(pass, ObjFile, nullptr, FileType)) {
            llvm::errs() << "TheTargetMachine can't emit a file of this type";
            return 1;
        }
    
        // Execute the passes to generate the object file
        pass.run(TheModule);
        ObjFile.flush();

        return true;
    } else {
        llvm::errs() << "Tipo de saída inválido: " << output_type << "\n";
        return false;
    }
}


int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s [--out=<object|ir>=<out_file>] [-o executable] [files...]\n", argv[0]);
        return 1;
    }

    if (argc > 1 && std::string(argv[1]) == "--repl") {
        startREPL();
        return 0;
    }

    ArgParseResult result = arg_parse(argc, argv);
    
    std::string output_type = "jit";
    std::string output_file = "output";

    for (int i = 0; i < result.named_count; i++) {
        std::cout << result.named_args[i].flag << "\n";
        if (strcmp(result.named_args[i].flag, "--out") == 0) {
            std::string value = result.named_args[i].value;
            size_t pos = value.find('=');
            if (pos != std::string::npos) {
                output_type = value.substr(0, pos);
                output_file = value.substr(pos + 1);
            } else {
                output_type = value;
            }
        }
    }

    FILE *sourceFile;
    if (!fopen_safe(sourceFile, argv[1], "r")) {
        fprintf(stderr, "Error opening file '%s'\n", argv[1]);
        return 1;
    }

    int count = 0;
    Token *tokens = tokenize(sourceFile, argv[1], &count);

    printf("Lexical Analysis:\n");
    printf("-----------------\n");

    for (int i = 0; i < count; i++) {
        Token token = tokens[i];
        printf("Line %d, Column %d-%d: %s \"%s\"\n",
               token.line,
               token.column_start,
               token.column_end,
               getTokenTypeName(token.type),
               token.lexeme);
    }

    Parser parser = parser_new();
    AstNode *ast = produce_ast(&parser, tokens, count, false);

    printf("\nParsing:\n");
    printf("-----------------\n");
    print_ast(ast);

    printf("\nGeneration:\n");
    printf("-----------------\n");

    // Initialize LLVM target-related components (needed to generate machine code)
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    // Create the LLVM context, module, and IR builder
    llvm::LLVMContext TheContext;
    auto TheModule = std::make_unique<llvm::Module>("GalaxyJIT", TheContext);
    llvm::IRBuilder<llvm::NoFolder> Builder(TheContext);

    // Create the global scope
    enter_scope();

    // Generate IR from the AST
    std::vector<llvm::Value*> values = generate_ir(ast, TheContext, *TheModule, Builder);
    // Print out the generated LLVM IR for debugging
    for (size_t i = 0; i < values.size(); ++i) {
        if (values[i]) {
            values[i]->print(llvm::errs());  // Print IR to error stream
            std::cout << "\n";
        } else {
            std::cerr << "Valor IR nulo encontrado no índice " << i << "\n";
        }
    }

    std::cout << output_type << "\n";
    std::cout << output_file << "\n";

    if (output_type != "jit") {
        generate_output(*TheModule, output_type, output_file);
    } else {
        auto JIT = llvm::orc::LLJITBuilder().create();
        if (!JIT) {
            llvm::errs() << "Erro ao criar o JIT: " << llvm::toString(JIT.takeError()) << "\n";
            free_ast_node(ast);
            freeTokens(tokens, count);
            fclose(sourceFile);
            return 1;
        }

        std::vector<std::string> modules {
            "libs/std.ll"
        };

        for (const auto &file : modules) {
            llvm::SMDiagnostic Err;

            // Carregar o módulo
            auto Module = llvm::parseIRFile(file, Err, TheContext);
            if (!Module) {
                llvm::errs() << "Erro ao carregar o arquivo " << file << ": " << Err.getMessage() << "\n";
                continue; // Pula para o próximo módulo
            }

            // Adicionar o módulo ao JIT
            if (auto Err = (*JIT)->addIRModule(llvm::orc::ThreadSafeModule(std::move(Module), std::make_unique<llvm::LLVMContext>()))) {
                llvm::errs() << "Erro ao adicionar o módulo " << file << " ao JIT: " << llvm::toString(std::move(Err)) << "\n";
                continue; // Pula para o próximo módulo
            }
        }

        if (auto Err = (*JIT)->addIRModule(llvm::orc::ThreadSafeModule(std::move(TheModule), std::make_unique<llvm::LLVMContext>()))) {
            llvm::errs() << "Erro ao adicionar o módulo IR ao JIT: " << llvm::toString(std::move(Err)) << "\n";
            free_ast_node(ast);
            freeTokens(tokens, count);
            fclose(sourceFile);
            return 1;
        }

        auto Func = (*JIT)->lookup("main");
        if (!Func) {
            llvm::errs() << "Erro ao buscar a função: " << llvm::toString(Func.takeError()) << "\n";
            free_ast_node(ast);
            freeTokens(tokens, count);
            fclose(sourceFile);
            return 1;
        }

        auto MainFunc = (int (*)())(Func->getValue());
        MainFunc();
    }

    free_ast_node(ast);
    freeTokens(tokens, count);
    fclose(sourceFile);
    return 0;
}