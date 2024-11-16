#include "lexer.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static FILE *src;
static int currentChar;
static int line = 1;
static int col = 1;
static int position = 0;  
static char *filename = NULL;

static Token *tokens = NULL;
static int tokenCount = 0;

void initLexer(FILE *source, const char *file) {
    src = source;
    filename = strdup(file);  
    currentChar = fgetc(src);
}

static void skipWhitespace() {
    while (isspace(currentChar)) {
        if (currentChar == '\n') {
            line++;
            col = 1;
        }
        currentChar = fgetc(src);
        col++;
        position++;  
    }
}

void addToken(TokenType type, const char *lexeme) {
    tokens = realloc(tokens, sizeof(Token) * (tokenCount + 1));
    Token *newToken = &tokens[tokenCount];

    newToken->type = type;
    newToken->lexeme = strdup(lexeme);  // Alocar memória dinamicamente para o lexeme
    newToken->line = line;
    newToken->column_start = col;
    newToken->position_start = position;
    newToken->position_end = position + strlen(lexeme);
    newToken->filename = strdup(filename);  // Alocar memória para o nome do arquivo
    newToken->message = strdup("");  // Inicializar message como string vazia

    tokenCount++;
}

Token getNextToken() {
    skipWhitespace();

    if (currentChar == EOF) {
        return (Token){TOKEN_EOF, strdup(""), line, col, position, position, strdup(filename), strdup("")};
    }

    if (isdigit(currentChar)) {
        char buffer[256];
        int i = 0;
        while (isdigit(currentChar)) {
            buffer[i++] = currentChar;
            currentChar = fgetc(src);
            col++;
            position++;
        }
        buffer[i] = '\0';
        return (Token){TOKEN_INT, strdup(buffer), line, col, position, position + i, strdup(filename), strdup("")};
    }

    if (isalpha(currentChar)) {
        char buffer[256];
        int i = 0;
        while (isalnum(currentChar) || currentChar == '_') {
            buffer[i++] = currentChar;
            currentChar = fgetc(src);
            col++;
            position++;
        }
        buffer[i] = '\0';

        if (strcmp(buffer, "package") == 0) return (Token){TOKEN_PACKAGE, strdup(buffer), line, col, position, position + i, strdup(filename), strdup("")};
        if (strcmp(buffer, "import") == 0) return (Token){TOKEN_IMPORT, strdup(buffer), line, col, position, position + i, strdup(filename), strdup("")};
        if (strcmp(buffer, "def") == 0) return (Token){TOKEN_DEF, strdup(buffer), line, col, position, position + i, strdup(filename), strdup("")};
        if (strcmp(buffer, "return") == 0) return (Token){TOKEN_RETURN, strdup(buffer), line, col, position, position + i, strdup(filename), strdup("")};
        return (Token){TOKEN_IDENTIFIER, strdup(buffer), line, col, position, position + i, strdup(filename), strdup("")};
    }


    switch (currentChar) {
        case '(':
            currentChar = fgetc(src);
            col++;
            position++;  
            return (Token){TOKEN_LPAREN, "(", line, col, position, position + 1, filename, ""};
        case ')':
            currentChar = fgetc(src);
            col++;
            position++;  
            return (Token){TOKEN_RPAREN, ")", line, col, position, position + 1, filename, ""};
        case ',':
            currentChar = fgetc(src);
            col++;
            position++;  
            return (Token){TOKEN_COMMA, ",", line, col, position, position + 1, filename, ""};
        case '+':
            currentChar = fgetc(src);
            col++;
            position++;  
            return (Token){TOKEN_PLUS, "+", line, col, position, position + 1, filename, ""};
        case '-':
            currentChar = fgetc(src);
            col++;
            position++;  
            return (Token){TOKEN_MINUS, "-", line, col, position, position + 1, filename, ""};
        case '*':
            currentChar = fgetc(src);
            col++;
            position++;  
            return (Token){TOKEN_MUL, "*", line, col, position, position + 1, filename, ""};
        case '/':
            currentChar = fgetc(src);
            col++;
            position++;  
            return (Token){TOKEN_DIV, "/", line, col, position, position + 1, filename, ""};
        case '^':
            currentChar = fgetc(src);
            col++;
            position++;  
            return (Token){TOKEN_POWER, "^", line, col, position, position + 1, filename, ""};
        case '%':
            currentChar = fgetc(src);
            col++;
            position++;  
            return (Token){TOKEN_MODULUS, "%", line, col, position, position + 1, filename, ""};
        case ':':
            currentChar = fgetc(src);
            col++;
            position++;  
            if (currentChar == '=') {
                currentChar = fgetc(src);
                col++;
                position++;  
                return (Token){TOKEN_ASSIGN, ":=", line, col, position, position + 2, filename, ""};
            }
            break;
        default:
            currentChar = fgetc(src);
            col++;
            position++;  
            break;
    }

    return (Token){TOKEN_UNKNOWN, "", line, col, position, position, filename, "Unknown character"};
}

Token* tokenize(FILE *source, const char *file, int *outTokenCount) {
    initLexer(source, file);
    Token token;
    tokenCount = 0;

    while ((token = getNextToken()).type != TOKEN_EOF) {
        addToken(token.type, token.lexeme);
    }

    *outTokenCount = tokenCount;
    return tokens;
}
