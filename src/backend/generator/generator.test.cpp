#include <iostream>
#include <exception>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>

#include "backend/generator/generate_ir.hpp"

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

int main(int argc, char **argv) {
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
    AstNode *ast = produce_ast(&parser, tokens, count);

    printf("\nParsing:\n");
    printf("-----------------\n");
    print_ast(ast);

    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::LLVMContext Context;
    llvm::Module Module("galaxy_module", Context);
    llvm::IRBuilder<> Builder(Context);

    std::vector<llvm::Value*> values = generate_ir(ast, Context, Module, Builder);

    for (size_t i = 0; i < values.size(); ++i) {
        if (values[i]) {
            values[i]->print(llvm::errs());
            std::cout << "\n";
        } else {
            std::cerr << "Valor IR nulo encontrado no índice " << i << "\n";
        }
    }

    std::string error;
    if (llvm::verifyModule(Module, &llvm::errs())) {
        std::cerr << "O módulo LLVM contém erros!\n";
        return 1;
    }

    std::string filename = "generated_ir.ll";
    std::error_code EC;
    llvm::raw_fd_ostream dest(filename, EC);

    if (EC) {
        std::cerr << "Erro ao abrir o arquivo para escrita: " << EC.message() << std::endl;
        return 1;
    }

    Module.print(dest, nullptr);

    std::cout << "IR escrito no arquivo " << filename << "\n";

    free_ast_node(ast);
    freeTokens(tokens, count);
    fclose(sourceFile);
    return 0;
}
