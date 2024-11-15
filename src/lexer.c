#include "lexer.h"
#include <ctype.h>
#include <string.h>


static FILE *src;
static int currentChar;


void initLexer(FILE *source) {
    src = source;
    currentChar = fgetc(src);
}


static void skipWhitespace() {
    while (isspace(currentChar)) {
        currentChar = fgetc(src);
    }
}

static Token createToken(TokenType type, const char *lexeme) {
    Token token;
    token.type = type;
    strcpy(token.lexeme, lexeme);
    return token;
}


Token getNextToken() {
    skipWhitespace();

    if (currentChar == EOF) {
        return createToken(TOKEN_EOF, "");
    }

    if (isdigit(currentChar)) {
        char buffer[256];
        int i = 0;
        while (isdigit(currentChar)) {
            buffer[i++] = currentChar;
            currentChar = fgetc(src);
        }
        buffer[i] = '\0';
        return createToken(TOKEN_INT, buffer);
    }

    if (isalpha(currentChar)) {
        char buffer[256];
        int i = 0;
        while (isalnum(currentChar)) {
            buffer[i++] = currentChar;
            currentChar = fgetc(src);
        }
        buffer[i] = '\0';
        if (strcmp(buffer, "package") == 0) return createToken(TOKEN_PACKAGE, buffer);
        if (strcmp(buffer, "import") == 0) return createToken(TOKEN_IMPORT, buffer);
        if (strcmp(buffer, "def") == 0) return createToken(TOKEN_DEF, buffer);
        if (strcmp(buffer, "return") == 0) return createToken(TOKEN_RETURN, buffer);
        return createToken(TOKEN_IDENTIFIER, buffer);
    }

    switch (currentChar) {
        case '(':
            currentChar = fgetc(src);
            return createToken(TOKEN_LPAREN, "(");
        case ')':
            currentChar = fgetc(src);
            return createToken(TOKEN_RPAREN, ")");
        case ',':
            currentChar = fgetc(src);
            return createToken(TOKEN_COMMA, ",");
        case '+': case '*': case '/': {
            char op = currentChar;
            currentChar = fgetc(src);
            char buffer[2] = {op, '\0'};
            return createToken(TOKEN_OPERATOR, buffer);
        }
        case '-': {
            int nextChar = fgetc(src);
            if (nextChar == '>') {
                currentChar = fgetc(src); 
                return createToken(TOKEN_ARROW, "->");
            } else {
                ungetc(nextChar, src); 
                char buffer[2] = {'-', '\0'};
                currentChar = fgetc(src);
                return createToken(TOKEN_OPERATOR, buffer);
            }
        }
        case ':':
            currentChar = fgetc(src);
            if (currentChar == '=') {
                currentChar = fgetc(src);
                return createToken(TOKEN_ASSIGN, ":=");
            }
            break;
        default:
            
            currentChar = fgetc(src);
            break;
    }


    if (currentChar == EOF) {
        return createToken(TOKEN_EOF, "");
    }

    return createToken(TOKEN_EOF, "");
}
