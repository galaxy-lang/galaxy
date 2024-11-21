#include <stdio.h>
#include <stdlib.h>
#include "../../../include/lexer/core.h"
#include "../../../include/utils.h"
#include "../../../include/lexer/freeTokens.h"

void freeTokens(Token *tokens, int tokenCount) {
    if (tokens != NULL) {
        for (int i = 0; i < tokenCount; i++) {
            free(tokens[i].lexeme);
            free(tokens[i].message);
        }
        free(tokens);
    }
}


const char* getTokenTypeName(TokenType type) {
    switch (type) {
        case TOKEN_EOF:        return "EOF";
        case TOKEN_LT:         return "LT";
        case TOKEN_GT:         return "GT";
        case TOKEN_EQUAL:      return "EQUAL";
        case TOKEN_IF:         return "IF";
        case TOKEN_ELIF:       return "ELIF";
        case TOKEN_ELSE:       return "ELSE";
        case TOKEN_SWITCH:     return "SWITCH";
        case TOKEN_CASE:       return "CASE";
        case TOKEN_DEFAULT:    return "DEFAULT";
        case TOKEN_ELLIPSIS:   return "ELIPSIS";
        case TOKEN_ASTERISK:   return "ASTERISK";
        case TOKEN_LEQUAL:     return "LEQUAL";
        case TOKEN_GEQUAL:     return "GEQUAL";
        case TOKEN_INT:        return "INTEGER";
        case TOKEN_DECIMAL:    return "DECIMAL";
        case TOKEN_STRING:     return "STRING";
        case TOKEN_BOOL:       return "BOOLEAN";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_PACKAGE:    return "PACKAGE";
        case TOKEN_IMPORT:     return "IMPORT";
        case TOKEN_DEF:        return "DEF";
        case TOKEN_RETURN:     return "RETURN";
        case TOKEN_LPAREN:     return "LPAREN";
        case TOKEN_RPAREN:     return "RPAREN";
        case TOKEN_COLON:      return "COLON";
        case TOKEN_COMMA:      return "COMMA";
        case TOKEN_DOT:        return "DOT";
        case TOKEN_PLUS:       return "PLUS";
        case TOKEN_MINUS:      return "MINUS";
        case TOKEN_MUL:        return "MULTIPLY";
        case TOKEN_DIV:        return "DIVIDE";
        case TOKEN_POWER:      return "POWER";
        case TOKEN_MODULUS:    return "MODULUS";
        case TOKEN_ASSIGN:     return "ASSIGN";
        case TOKEN_ARROW:      return "ARROW";
        case TOKEN_UNKNOWN:    return "UNKNOWN";
        default:               return "INVALID TOKEN";
    }
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

    freeTokens(tokens, count);
    fclose(sourceFile);
    return 0;
}
