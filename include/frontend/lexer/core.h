#ifndef CORE_H
#define CORE_H

#include <stdio.h>

typedef enum {
    TOKEN_AND,
    TOKEN_ARROW,
    TOKEN_ASSIGN,
    TOKEN_AT,
    TOKEN_BITWISE_AND,
    TOKEN_BITWISE_NOT,
    TOKEN_BITWISE_OR,
    TOKEN_BITWISE_XOR,
    TOKEN_CASE,
    TOKEN_CBRACE,
    TOKEN_COLON,
    TOKEN_COMMA,
    TOKEN_CONST,
    TOKEN_CPAREN,
    TOKEN_DECREMENT,
    TOKEN_DEF,
    TOKEN_DEFAULT,
    TOKEN_DIV,
    TOKEN_DOT,
    TOKEN_ELIF,
    TOKEN_ELLIPSIS,
    TOKEN_ELSE,
    TOKEN_END,
    TOKEN_EQUAL,
    TOKEN_EOF,
    TOKEN_FALSE,
    TOKEN_FOR,
    TOKEN_GEQUAL,
    TOKEN_GT,
    TOKEN_IDENTIFIER,
    TOKEN_IF,
    TOKEN_INCREMENT,
    TOKEN_IMPORT,
    TOKEN_IRANGE,
    TOKEN_LEQUAL,
    TOKEN_LT,
    TOKEN_MINUS,
    TOKEN_MODULUS,
    TOKEN_MUL,
    TOKEN_NEQUAL,
    TOKEN_NOT,
    TOKEN_NUMBER,
    TOKEN_OBRACE,
    TOKEN_OPAREN,
    TOKEN_OR,
    TOKEN_PACKAGE,
    TOKEN_PLUS,
    TOKEN_POWER,
    TOKEN_QUESTION_MARK,
    TOKEN_RANGE,
    TOKEN_RETURN,
    TOKEN_SEMICOLON,
    TOKEN_SHIFT_LEFT,
    TOKEN_SHIFT_RIGHT,
    TOKEN_STRING,
    TOKEN_SWITCH,
    TOKEN_TRUE,
    TOKEN_TYPE_BOOL,
    TOKEN_TYPE_DECIMAL,
    TOKEN_TYPE_DOUBLE,
    TOKEN_TYPE_FLOAT,
    TOKEN_TYPE_INT,
    TOKEN_TYPE_STRING,
    TOKEN_TYPE_VOID,
    TOKEN_TYPE_TUPLE,
    TOKEN_TYPE_LIST,
    TOKEN_OBRACKET,
    TOKEN_CBRACKET,
    TOKEN_UNKNOWN
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
