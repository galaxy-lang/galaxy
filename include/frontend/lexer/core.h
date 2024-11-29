#ifndef CORE_H
#define CORE_H

#include <stdio.h>

typedef enum {
    TOKEN_UNKNOWN,
    TOKEN_NOT,
    TOKEN_BITWISE_NOT,
    TOKEN_INCREMENT,
    TOKEN_DECREMENT,
    TOKEN_PACKAGE,
    TOKEN_IMPORT,
    TOKEN_DEF,
    TOKEN_RETURN,
    TOKEN_IDENTIFIER,
    TOKEN_INT,
    TOKEN_DECIMAL,
    TOKEN_STRING,
    TOKEN_SEMICOLON,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_ELLIPSIS,
    TOKEN_IF,
    TOKEN_FOR,
    TOKEN_ELSE,
    TOKEN_ELIF,
    TOKEN_SWITCH,
    TOKEN_CASE,
    TOKEN_DEFAULT,
    TOKEN_OPAREN,
    TOKEN_CPAREN,
    TOKEN_COMMA,
    TOKEN_COLON,
    TOKEN_ARROW,
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_POWER,
    TOKEN_MODULUS,
    TOKEN_ASSIGN,
    TOKEN_DOT,
    TOKEN_LT,
    TOKEN_GT,
    TOKEN_EQUAL,
    TOKEN_RANGE,
    TOKEN_LEQUAL,
    TOKEN_GEQUAL,
    TOKEN_OBRACE,
    TOKEN_CBRACE,
    TOKEN_BITWISE_OR,
    TOKEN_BITWISE_XOR,
    TOKEN_BITWISE_AND,
    TOKEN_SHIFT_LEFT,
    TOKEN_SHIFT_RIGHT,
    TOKEN_END,
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
void addToken(TokenType type, const char *lexeme, int line, int col_s, int col_e, int pos_s, int pos_e, const char *filename, char *message);
void skipWhitespace();
Token getNextToken();
Token *tokenize(FILE *sourceFile, const char *fileName, int *count);

#endif  // CORE_H
