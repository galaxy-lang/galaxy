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

int startREPL() {
    // Inicializar LLVM
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    // Criar o JIT fora do loop
    auto JIT = llvm::orc::LLJITBuilder().create();
    if (!JIT) {
        llvm::errs() << "Erro ao inicializar LLJIT: " << llvm::toString(JIT.takeError()) << "\n";
        return 1;
    }

    enter_scope();

    std::cout << "(GalaxyJIT REPL v1.0.0) Enter code below ('exit' to quit):\n";

    std::string Line;
    while (true) {
        std::cout << ">>> ";
        std::getline(std::cin, Line);
        if (Line.empty()) continue;
        if (Line == "exit") break;

        // Criar um novo contexto, módulo e builder para cada iteração
        llvm::LLVMContext TheContext;
        auto TheModule = std::make_unique<llvm::Module>("GalaxyJIT", TheContext);
        llvm::IRBuilder<llvm::NoFolder> Builder(TheContext);

        // Criar um ResourceTracker para gerenciar o módulo desta iteração
        auto ResourceTracker = (*JIT)->getMainJITDylib().createResourceTracker();

        FILE *tempFile = fopen("repl.glx", "w");
        if (!tempFile) {
            std::cerr << "Erro ao abrir arquivo temporário.\n";
            return 1;
        }

        fprintf(tempFile, "%s\n", Line.c_str());
        fclose(tempFile);

        tempFile = fopen("repl.glx", "r");
        int count = 0;
        Token *tokens = tokenize(tempFile, "repl.glx", &count);
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
        llvm::Function *mainFunc = llvm::Function::Create(mainFuncType, llvm::Function::ExternalLinkage, "entry.main", *TheModule);

        llvm::BasicBlock *entryBlock = llvm::BasicBlock::Create(TheContext, "entry", mainFunc);
        Builder.SetInsertPoint(entryBlock);

        generate_ir(ast, TheContext, *TheModule, Builder);

        Builder.CreateRetVoid();

        // Verificar o módulo
        std::string errorMsg;
        llvm::raw_string_ostream errorStream(errorMsg);
        if (llvm::verifyModule(*TheModule, &errorStream)) {
            llvm::errs() << "Erro ao verificar módulo:\n" << errorStream.str() << "\n";
            free_ast_node(ast);
            freeTokens(tokens, count);
            continue;
        }

        // Adicionar o módulo ao JIT (sem usar o ResourceTracker diretamente)
        if (auto Err = (*JIT)->addIRModule(llvm::orc::ThreadSafeModule(std::move(TheModule), std::make_unique<llvm::LLVMContext>()))) {
            llvm::errs() << "Erro ao adicionar o módulo IR ao JIT: " << llvm::toString(std::move(Err)) << "\n";
            free_ast_node(ast);
            freeTokens(tokens, count);
            return 1;
        }

        // Executar a função entry.main
        auto Func = (*JIT)->lookup("entry.main");
        if (!Func) {
            llvm::errs() << "Erro: entry.main não encontrado.\n";
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