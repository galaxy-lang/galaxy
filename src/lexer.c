#include "../include/lexer.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

Token *tokens = NULL;
int tokenCount = 0;
FILE *src;
char currentChar;
int line = 1;
int col = 1;
int position = 0;
const char *filename = NULL;

void initLexer(FILE *source, const char *file) {
    src = source;
    filename = file;
    currentChar = fgetc(src);
}

void skipWhitespace() {
    while (isspace(currentChar)) {
        if (currentChar == '\n') {
            line++;
            col = 1;
        } else if (currentChar == '\t') {
            col += 4;
        } else {
            col++;
        }
        currentChar = fgetc(src);
        position++;
    }
}


void addToken(TokenType type, const char *lexeme) {
    Token *newTokens = realloc(tokens, sizeof(Token) * (tokenCount + 1));
    if (!newTokens) {
        fprintf(stderr, "Error allocating memory for tokens.\n");
        exit(EXIT_FAILURE);
    }
    tokens = newTokens;

    Token *newToken = &tokens[tokenCount];
    newToken->type = type;
    newToken->lexeme = strdup(lexeme);
    newToken->line = line;
    newToken->column_start = col - strlen(lexeme);
    newToken->column_end = col - 1;
    newToken->position_start = position - strlen(lexeme);
    newToken->position_end = position - 1;
    newToken->filename = filename;
    newToken->message = strdup("");
    tokenCount++;
}


int isAlpha(char c) {
    return isalpha(c) || c == '_';
}


int isAlphaNumeric(char c) {
    return isalnum(c) || c == '_';
}

TokenType matchKeyword(const char *lexeme) {
    if (strcmp(lexeme, "package") == 0) return TOKEN_PACKAGE;
    if (strcmp(lexeme, "import") == 0) return TOKEN_IMPORT;
    if (strcmp(lexeme, "def") == 0) return TOKEN_DEF;
    if (strcmp(lexeme, "return") == 0) return TOKEN_RETURN;

    return TOKEN_IDENTIFIER;
}

Token getNextToken() {
    skipWhitespace();

    if ((int)currentChar == EOF) {
        return (Token){TOKEN_EOF, strdup("EOF"), line, col, col, position, position, filename, strdup("")};
    }

    if (isAlpha(currentChar)) {
        char buffer[256];
        int i = 0;

        while (isAlphaNumeric(currentChar)) {
            if (i >= 255) {
                fprintf(stderr, "Identifier too long in row %d, column %d.\n", line, col);
                exit(EXIT_FAILURE);
            }
            buffer[i++] = currentChar;
            currentChar = fgetc(src);
            col++;
            position++;
        }

        buffer[i] = '\0';
        TokenType type = matchKeyword(buffer);
        return (Token){type, strdup(buffer), line, col - i, col - 1, position - i, position - 1, filename, strdup("")};
    }

    if (isdigit(currentChar)) {
    char buffer[256];
    int i = 0;
    int isDecimal = 0;

    while (isdigit(currentChar) || currentChar == '.') {
        if (currentChar == '.') {
            if (isDecimal) break; 
            isDecimal = 1;
        }
        if (i >= 255) {
            fprintf(stderr, "Number too long in row %d, column %d.\n", line, col);
            exit(EXIT_FAILURE);
        }
        buffer[i++] = currentChar;
        currentChar = fgetc(src);
        col++;
        position++;
    }

    buffer[i] = '\0';
    return (Token){
        isDecimal ? TOKEN_DECIMAL : TOKEN_INT,
        strdup(buffer),
        line, col - i, col - 1, position - i, position - 1,
        filename, strdup("")
    };
}

    switch (currentChar) {
        case '(': currentChar = fgetc(src); col++; position++; return (Token){TOKEN_LPAREN, strdup("("), line, col - 1, col - 1, position - 1, position - 1, filename, strdup("")};
        case ')': currentChar = fgetc(src); col++; position++; return (Token){TOKEN_RPAREN, strdup(")"), line, col - 1, col - 1, position - 1, position - 1, filename, strdup("")};
        case ',': currentChar = fgetc(src); col++; position++; return (Token){TOKEN_COMMA, strdup(","), line, col - 1, col - 1, position - 1, position - 1, filename, strdup("")};
        case '+': currentChar = fgetc(src); col++; position++; return (Token){TOKEN_PLUS, strdup("+"), line, col - 1, col - 1, position - 1, position - 1, filename, strdup("")};
        case '*': currentChar = fgetc(src); col++; position++; return (Token){TOKEN_MUL, strdup("*"), line, col - 1, col - 1, position - 1, position - 1, filename, strdup("")};
        case '/': currentChar = fgetc(src); col++; position++; return (Token){TOKEN_DIV, strdup("/"), line, col - 1, col - 1, position - 1, position - 1, filename, strdup("")};
      
        case ':':
            currentChar = fgetc(src);
            col++;
            position++;
            if (currentChar == '=') {
                currentChar = fgetc(src);
                col++;
                position++;
                return (Token){TOKEN_ASSIGN, strdup(":="), line, col - 2, col - 1, position - 2, position - 1, filename, strdup("")};
            } else {
                return (Token){TOKEN_COLON, strdup(":"), line, col - 1, col - 1, position - 1, position - 1, filename, strdup("")};
            }
      
        case '.':
            currentChar = fgetc(src);
            col++;
            position++;
            return (Token){TOKEN_DOT, strdup("."), line, col - 1, col - 1, position - 1, position - 1, filename, strdup("")};
      
        case '-':
            currentChar = fgetc(src);
            col++;
            position++;
      
            if (currentChar == '>') {
                currentChar = fgetc(src);
                col++;
                position++;
                return (Token){TOKEN_ARROW, strdup("->"), line, col - 2, col - 1, position - 2, position - 1, filename, strdup("")};
            }
            return (Token){TOKEN_MINUS, strdup("-"), line, col - 1, col - 1, position - 1, position - 1, filename, strdup("")};

    default:
    fprintf(stderr, "Unexpected character: '%c' in line %d, column %d.\n", currentChar, line, col);
    currentChar = fgetc(src);
    col++;
    position++;
    return (Token){TOKEN_UNKNOWN, strdup(""), line, col - 1, col - 1, position - 1, position - 1, filename, strdup("Caractere inesperado")};

    }
}

Token *tokenize(FILE *sourceFile, const char *fileName, int *count) {
    initLexer(sourceFile, fileName);
    Token token;
    tokenCount = 0;
    tokens = NULL;

    do {
        token = getNextToken();
        if (token.type != TOKEN_EOF) {
            addToken(token.type, token.lexeme);
        }
    } while (token.type != TOKEN_EOF);

    *count = tokenCount;
    return tokens;
}
