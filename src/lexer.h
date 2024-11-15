#ifndef LEXER_H
#define LEXER_H

#include <stdio.h> // Necessário para o tipo FILE

typedef enum {
    TOKEN_EOF,
    TOKEN_INT,
    TOKEN_DECIMAL,
    TOKEN_STRING,
    TOKEN_BOOL,
    TOKEN_IDENTIFIER,
    TOKEN_KEYWORD,
    TOKEN_OPERATOR,
    TOKEN_ASSIGN,
    TOKEN_RETURN,
    TOKEN_PACKAGE,
    TOKEN_IMPORT,
    TOKEN_DEF,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_COMMA,
    TOKEN_SEMICOLON,
    TOKEN_ARROW
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[256];
} Token;

void initLexer(FILE *source);
Token getNextToken();

#endif
