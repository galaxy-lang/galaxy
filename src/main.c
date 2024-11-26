#include <stdio.h>
#include <stdlib.h>
#include "../include/lexer/core.h"
#include "../include/parser/core.h"
#include "../include/utils.h"

/**
 * @brief Frees the memory allocated for an array of tokens.
 *
 * This function iterates over the array of tokens and frees the memory allocated for each token's lexeme and message.
 * After freeing the memory for each token, it frees the memory for the array of tokens itself.
 *
 * @param tokens The array of tokens to be freed.
 * @param tokenCount The number of tokens in the array.
 */
void freeTokens(Token *tokens, int tokenCount) {
    if (tokens != NULL) {
        for (int i = 0; i < tokenCount; i++) {
            FREE_S(tokens[i].lexeme);  
            FREE_S(tokens[i].message); 
        }
        FREE_S(tokens);  
    }
}

/**
 * @brief Entry point for the program.
 *
 * Processes a source file by tokenizing its contents, generating an abstract syntax tree (AST), 
 * and cleaning up allocated resources. 
 *
 * @param argc Number of command-line arguments.
 * @param argv Command-line arguments: program name and source file path.
 * @return 0 on success, or a non-zero error code on failure.
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
    
    Parser parser = parser_new();
    AstNode *ast = produce_ast(&parser, tokens, count);

    free_ast_node(ast);
    freeTokens(tokens, count);
    fclose(sourceFile);
    return 0;
}
