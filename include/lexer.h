#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

#ifdef _WIN32
#define strdup _strdup
#endif

typedef enum {
    TOKEN_UNKNOWN,
    TOKEN_PACKAGE,
    TOKEN_IMPORT,
    TOKEN_DEF,
    TOKEN_RETURN,
    TOKEN_IDENTIFIER,
    TOKEN_INT,
    TOKEN_DECIMAL,
    TOKEN_STRING,
    TOKEN_BOOL,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_COMMA,
    TOKEN_COLON,
    TOKEN_SEMICOLON,
    TOKEN_ARROW,
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_POWER,
    TOKEN_MODULUS,
    TOKEN_ASSIGN,
    TOKEN_OPERATOR,
    TOKEN_NEWLINE,
    TOKEN_TAB,
    TOKEN_DOT,
    TOKEN_KEYWORD,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char *lexeme;
    int line;
    int column_start;
    int column_end;
    int position_start;
    int position_end;
    const char *filename;
    char *message;
} Token;

extern Token *tokens;
extern int tokenCount;
extern FILE *src;
extern char currentChar;
extern int line;
extern int col;
extern int position;
extern const char *filename;

void initLexer(FILE *source, const char *file);
void addToken(TokenType type, const char *lexeme);
void skipWhitespace();
Token getNextToken();
Token *tokenize(FILE *sourceFile, const char *fileName, int *count);
void freeTokens();

#endif  // LEXER_H
