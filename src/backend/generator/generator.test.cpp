#include <stdio.h>
#include <stdlib.h>
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

/**
 * @brief The entry point of the program for lexical analysis and parsing.
 *
 * This function performs lexical analysis on the input source file, tokenizing it and printing 
 * information about each token. It then parses the tokens to produce an abstract syntax tree (AST), 
 * printing the structure of the AST. Afterward, it frees the allocated memory for tokens and the AST.
 *
 * @param argc The number of command-line arguments passed to the program.
 * @param argv The array of command-line arguments. The second argument should be the source file.
 * @return An integer status code: 0 on success, 1 on failure.
 */
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

    free_ast_node(ast);
    freeTokens(tokens, count);
    fclose(sourceFile);
    return 0;
}
