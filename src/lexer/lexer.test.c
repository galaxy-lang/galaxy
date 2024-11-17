#include <stdio.h>
#include <stdlib.h>
#include "../../include/lexer.h"

const char* getTokenTypeName(TokenType type) {
    switch (type) {
        case TOKEN_EOF:        return "EOF";
        case TOKEN_LT:         return "LT";
        case TOKEN_GT:         return "GT";
        case TOKEN_EQUAL:      return "EQUAL";
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
        case TOKEN_NEWLINE:    return "NEWLINE";
        case TOKEN_TAB:        return "TAB";
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

    initLexer(sourceFile, argv[1]);

    Token token;
    printf("Lexical Analysis:\n");
    printf("-----------------\n");

    token = getNextToken();
		printf("Token: %-12s | Lexeme: '%s'\n", getTokenTypeName(token.type), token.lexeme);

    while (token.type != TOKEN_EOF){
        token = getNextToken();
        printf("Token: %-12s | Lexeme: '%s'\n", getTokenTypeName(token.type), token.lexeme);
    };

    fclose(sourceFile);
    return 0;
}
