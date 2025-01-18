#include <iostream>
#include <exception>
#include <vector>
#include <fstream>
#include <filesystem> 
#include <unordered_set>
#include <sstream>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/Orc/LLJIT.h>
#include <llvm/ExecutionEngine/Orc/ExecutionUtils.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>

#include "backend/generator/generate_ir.hpp"
#include "backend/generator/symbols/symbol_stack.hpp"

extern "C" {
    #include "frontend/lexer/core.h"
    #include "utils.h"
    #include "frontend/lexer/freeTokens.h"
    #include "frontend/ast/definitions.h"
    #include "frontend/parser/core.h"
    #include "frontend/parser/printer/print_ast.h"
    #include "frontend/getTokenTypeName.h"
    #include "frontend/freeTokens.h"
}

#include <iostream>
#include <fstream>
#include <string>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/ExecutionEngine/Orc/LLJIT.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/ExecutionEngine/Orc/ThreadSafeModule.h>
#include <llvm/ExecutionEngine/Orc/LLJIT.h>
#include <llvm/Support/TargetSelect.h>

// Função para adicionar o código de entrada (entry.main) e executar
void createMainFunc(llvm::Module &module, llvm::LLVMContext &context, llvm::IRBuilder<llvm::NoFolder> &builder) {
    llvm::FunctionType *mainFuncType = llvm::FunctionType::get(builder.getVoidTy(), false);
    llvm::Function *mainFunc = llvm::Function::Create(mainFuncType, llvm::Function::ExternalLinkage, "entry.main", module);

    llvm::BasicBlock *entryBlock = llvm::BasicBlock::Create(context, "entry", mainFunc);
    builder.SetInsertPoint(entryBlock);

    FILE *outFile = fopen("repl.glx", "r");

    int count = 0;
    Token *tokenArray = tokenize(outFile, "repl.glx", &count); // Passando FILE*, nome do arquivo e count

    // Gerar AST com base nos tokens
    Parser parser = parser_new();
    AstNode *ast = produce_ast(&parser, tokenArray, count, true); // Função fornecida por você
    if (!ast) {
        std::cerr << "Error: Invalid input or failed to parse AST.\n";
    }

    generate_ir(ast, context, module, builder);

    // Adicionar código gerado ao JIT
    builder.CreateRetVoid();
}

// Função para processar os nós AST
void processAstNode(AstNode *node, std::string &Line, FILE *outFile, std::stack<std::string> &declarationStack, std::vector<std::string>& codeLines) {
    if (node == nullptr) {
        std::cerr << "Error: Encountered null node!" << std::endl;
        return;
    }

    // Verificação usando switch para os tipos de nós
    switch (node->kind) {
        case NODE_EXTERN:
        case NODE_FUNCTION:
        case NODE_VARIABLE:
        case NODE_ASSIGNMENT:
            // Se for uma função, variável ou atribuição, empilhar a linha
            declarationStack.push(Line);
            // Armazenar o código no vetor para mais tarde
            codeLines.push_back(Line);
            break;
        default:
            // Se não for declaração nem atribuição, processa normalmente
            break;
    }
}

// Função para concatenar as declarações e escrevê-las no arquivo
void writeDeclarationsToFile(std::stack<std::string> &declarationStack, FILE *outFile) {
    std::vector<std::string> declarations;  // Vetor para armazenar as declarações

    // Empilhar as declarações no vetor
    while (!declarationStack.empty()) {
        declarations.push_back(declarationStack.top());
        declarationStack.pop();
    }

    // Concatenar as declarações em uma única string
    std::string allDeclarations;
    for (const auto& decl : declarations) {
        allDeclarations += decl + "\n";  // Adiciona um newline para separar as declarações
    }

    // Escrever a string concatenada no arquivo
    fprintf(outFile, "%s", allDeclarations.c_str());
}

// Função principal do REPL
int startREPL() {
    // Inicializar o LLVM e o JIT
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    std::unique_ptr<llvm::LLVMContext> Context = std::make_unique<llvm::LLVMContext>();
    auto JIT = llvm::orc::LLJITBuilder().create();
    if (!JIT) {
        llvm::errs() << "Failed to initialize LLJIT: " << llvm::toString(JIT.takeError()) << "\n";
        return 1;
    }

    std::cout << "(GalaxyJIT REPL v2.0.0) Enter code below ('exit' to quit):\n";

    llvm::IRBuilder<llvm::NoFolder> Builder(*Context);
    llvm::Module TheModule("REPLModule", *Context);

    std::string Line;

    std::stack<std::string> declarationStack; // Pilha para armazenar declarações e atribuições
    std::vector<std::string> codeLines; // Vetor para armazenar o código gerado

    while (true) {
        // Abrir arquivo para escrita das declarações
        FILE *outFile = fopen("repl.glx", "a");
        if (!outFile) {
            std::cerr << "Error opening file to write.\n";
            return 1;
        }

        std::cout << ">>> ";
        std::getline(std::cin, Line);
        if (Line.empty()) continue;
        if (Line == "exit") break;

        // 1. Escrever as declarações no arquivo
        writeDeclarationsToFile(declarationStack, outFile);

        // 2. Adicionar a linha atual ao vetor de código
        codeLines.push_back(Line);

        // 3. Adicionar a linha ao arquivo imediatamente

        // Concatenar todas as linhas do código em uma única string, separadas por '\n'
        std::string concatenatedCode = "";
        for (const auto& code : codeLines) {
            concatenatedCode += code + "\n";
        }

        fputs(concatenatedCode.c_str(), outFile);

        // 4. Gerar AST a partir da entrada do código concatenado
        int count = 0;
        Token *tokenArray = tokenize(outFile, "repl.glx", &count); // Passando FILE*, nome do arquivo e count

        // Gerar AST com base nos tokens
        Parser parser = parser_new();
        AstNode *ast = produce_ast(&parser, tokenArray, count, true); // Função fornecida por você
        if (!ast) {
            std::cerr << "Error: Invalid input or failed to parse AST.\n";
            continue;
        }

        if (ast->kind == NODE_PROGRAM) {
            ProgramNode* programNode = (ProgramNode*)ast->data;

            if (programNode->statement_count == 0) {
                continue;
            }

            // 5. Processar os statements da AST
            for (size_t i = 0; i < programNode->statement_count; ++i) {
                processAstNode(programNode->statements[i], Line, outFile, declarationStack, codeLines);
            }

            // 6. criar a função main
            createMainFunc(TheModule, *Context, Builder);

            // Adicionar módulos externos ao JIT (libs)
            std::vector<std::string> modules { "libs/std.ll" };
            for (const auto &file : modules) {
                llvm::SMDiagnostic Err;

                // Carregar o módulo
                auto Module = llvm::parseIRFile(file, Err, *Context);
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

            llvm::orc::ThreadSafeModule safeModule(std::make_unique<llvm::Module>("REPLModule", *Context), std::make_unique<llvm::LLVMContext>());
            (*JIT)->addIRModule(std::move(safeModule));

            // 7. Executar a função 'entry.main' após adicionar o módulo
            auto entryMainSymbol = (*JIT)->lookup("entry.main");
            if (entryMainSymbol) {
                auto *entryMainFunc = (void (*)())entryMainSymbol->getValue();
                entryMainFunc(); // Executa a função 'entry.main'
            } else {
                std::cerr << "Error: entry.main not found.\n";
            }
        }

        // Limpeza da AST e tokens após o processamento
        free_ast_node(ast);
        freeTokens(tokenArray, count);

        std::cout << "Processed input and updated stacks.\n";

        fclose(outFile); // Fechar o arquivo após a execução do REPL
    }

    return 0;
}


int main(int argc, char **argv) {
    if (argc > 1 && std::string(argv[1]) == "--repl") {
        startREPL();
        return 0;
    }

    if (argc < 2) {
        printf("Usage: %s <source_file>\n", argv[0]);
        return 1;
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

    // Verify the generated module for correctness
    std::string errorMsg;
    llvm::raw_string_ostream errorStream(errorMsg);
    if (llvm::verifyModule(*TheModule, &errorStream)) {
        llvm::errs() << "Erro ao verificar o módulo:\n" << errorStream.str();
        free_ast_node(ast);
        freeTokens(tokens, count);
        fclose(sourceFile);
        return 1;
    }

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

    free_ast_node(ast);
    freeTokens(tokens, count);
    fclose(sourceFile);
    return 0;
}