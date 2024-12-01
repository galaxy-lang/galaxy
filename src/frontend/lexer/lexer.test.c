#include <stdio.h>
#include <stdlib.h>
#include "frontend/lexer/core.h"
#include "frontend/lexer/freeTokens.h"
#include "utils.h"

/**
 * @brief Frees the memory allocated for an array of tokens.
 *
 * This function frees both the `lexeme` and `message` strings of each token, as well as the 
 * memory allocated for the array of tokens itself. It helps prevent memory leaks after token 
 * processing is complete.
 *
 * @param tokens The array of tokens to be freed.
 * @param tokenCount The number of tokens in the array.
 */
void freeTokens(Token *tokens, int tokenCount) {
    if (tokens != NULL) {
        for (int i = 0; i < tokenCount; i++) {
            free(tokens[i].lexeme);
            free(tokens[i].message);
        }
        free(tokens);
    }
}

/**
 * @brief Returns the name of a token type as a string.
 *
 * This function takes a `TokenType` and returns the corresponding string representation of the 
 * token type. It is useful for printing information about tokens during lexical analysis.
 *
 * @param type The type of the token (enum `TokenType`).
 * @return A string representing the name of the token type.
 */
const char* getTokenTypeName(TokenType type) {
    switch (type) {
        case TOKEN_EOF:         return "EOF";
        case TOKEN_DECREMENT:   return "DECREMENT";
        case TOKEN_INCREMENT:   return "INCREMENT";
        case TOKEN_NOT:         return "NOT";
        case TOKEN_STRING:      return "STRING";
        case TOKEN_BITWISE_NOT: return "BITWISE NOT";
        case TOKEN_FOR:         return "FOR";
        case TOKEN_LT:          return "LT";
        case TOKEN_GT:          return "GT";
        case TOKEN_EQUAL:       return "EQUAL";
        case TOKEN_IF:          return "IF";
        case TOKEN_ELIF:        return "ELIF";
        case TOKEN_ELSE:        return "ELSE";
        case TOKEN_SWITCH:      return "SWITCH";
        case TOKEN_CASE:        return "CASE";
        case TOKEN_DEFAULT:     return "DEFAULT";
        case TOKEN_SEMICOLON:   return "SEMICOLON";
        case TOKEN_END:         return "END";
        case TOKEN_ELLIPSIS:    return "ELIPSIS";
        case TOKEN_LEQUAL:      return "LEQUAL";
        case TOKEN_GEQUAL:      return "GEQUAL";
        case TOKEN_TYPE_INT:    return "TYPE INTEGER";
        case TOKEN_TYPE_DECIMAL:return "TYPE DECIMAL";
        case TOKEN_TYPE_STRING: return "TYPE STRING";
        case TOKEN_FALSE:       return "FALSE";
        case TOKEN_IDENTIFIER:  return "IDENTIFIER";
        case TOKEN_PACKAGE:     return "PACKAGE";
        case TOKEN_IMPORT:      return "IMPORT";
        case TOKEN_DEF:         return "DEF";
        case TOKEN_RETURN:      return "RETURN";
        case TOKEN_OPAREN:      return "OPAREN";
        case TOKEN_CPAREN:      return "CPAREN";
        case TOKEN_COLON:       return "COLON";
        case TOKEN_COMMA:       return "COMMA";
        case TOKEN_DOT:         return "DOT";
        case TOKEN_PLUS:        return "PLUS";
        case TOKEN_MINUS:       return "MINUS";
        case TOKEN_MUL:         return "MULTIPLY";
        case TOKEN_DIV:         return "DIVIDE";
        case TOKEN_POWER:       return "POWER";
        case TOKEN_MODULUS:     return "MODULUS";
        case TOKEN_ASSIGN:      return "ASSIGN";
        case TOKEN_ARROW:       return "ARROW";
        case TOKEN_NUMBER:      return "NUMBER";
        case TOKEN_BITWISE_OR:  return "BITWISE OR";
        case TOKEN_BITWISE_XOR: return "BITWISE XOR";
        case TOKEN_BITWISE_AND: return "BITWISE AND";
        case TOKEN_SHIFT_LEFT:  return "SHIFT LEFT";
        case TOKEN_SHIFT_RIGHT: return "SHIFT RIGHT";
        case TOKEN_UNKNOWN:     return "UNKNOWN";
        default:                return "INVALID TOKEN";
    }
}

/**
 * @brief The main entry point of the program for lexical analysis.
 *
 * This program reads a source file specified by the user, performs lexical analysis by tokenizing the contents of the file, 
 * and prints out the token details such as their type, position (line and column), and lexeme.
 *
 * The program expects the source file to be passed as an argument to the executable. If the file is successfully opened,
 * the program will tokenize the contents of the file and display each token's information. Afterward, it frees the allocated 
 * memory for the tokens and closes the file.
 *
 * @param argc The number of command-line arguments passed to the program.
 * @param argv The array of command-line arguments, where the second argument (argv[1]) is the path to the source file.
 * @return Returns 0 on successful execution, or 1 if the program fails to open the source file or if the file argument is missing.
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

    freeTokens(tokens, count);
    fclose(sourceFile);
    return 0;
}
