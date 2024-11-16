#include <stdio.h>
#include <stdlib.h>
#include "lexer.h" 


const char* getTokenTypeName(TokenType type) {
    switch (type) {
        case TOKEN_EOF:        return "EOF";
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
        case TOKEN_COMMA:      return "COMMA";
        case TOKEN_PLUS:       return "PLUS";
        case TOKEN_MINUS:      return "MINUS";
        case TOKEN_MUL:        return "MULTIPLY";
        case TOKEN_DIV:        return "DIVIDE";
        case TOKEN_POWER:      return "POWER";
        case TOKEN_MODULUS:    return "MODULUS";
        case TOKEN_ASSIGN:     return "ASSIGN";
        case TOKEN_SEMICOLON:  return "SEMICOLON";
        case TOKEN_ARROW:      return "ARROW";
        case TOKEN_KEYWORD:    return "KEYWORD";
        case TOKEN_OPERATOR:   return "OPERATOR";
        case TOKEN_UNKNOWN:    return "UNKNOWN";
        default:               return "INVALID TOKEN";
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <source_file>\n", argv[0]);
        return 1;
    }

    FILE *sourceFile = fopen(argv[1], "r");
    if (!sourceFile) {
        perror("Error opening file");
        return 1;
    }

    
    initLexer(sourceFile, argv[1]);

    Token token;
    printf("Lexical Analysis:\n");
    printf("-----------------\n");

    
    do {
        token = getNextToken();
        printf("Token: %-12s | Lexeme: '%s'\n", 
               getTokenTypeName(token.type), token.lexeme);
    } while (token.type != TOKEN_EOF);

    
    fclose(sourceFile);
    return 0;
}
