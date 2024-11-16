#ifndef LEXER_H
#define LEXER_H

#include <stdio.h> // necessario para o FILE

typedef enum {
    TOKEN_EOF,
    TOKEN_INT,
    TOKEN_DECIMAL,
    TOKEN_STRING,
    TOKEN_BOOL,
    TOKEN_IDENTIFIER,
    TOKEN_PACKAGE,
    TOKEN_IMPORT,
    TOKEN_DEF,
    TOKEN_RETURN,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_COMMA,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_POWER,
    TOKEN_MODULUS,
    TOKEN_ASSIGN,
    TOKEN_SEMICOLON, 
    TOKEN_ARROW,     
    TOKEN_KEYWORD,   
    TOKEN_OPERATOR,  
    TOKEN_UNKNOWN
} TokenType;


typedef struct {
    TokenType type;
    char *lexeme; 
    int line;
    int column_start;  
    int position_start;
    int position_end;
    char *filename;  // Alterado para ponteiro para char
    char *message;  // Alterado para ponteiro para char
    char *extra;     // Pode ser usado para outros dados, se necessário
} Token;

void addToken(TokenType type, const char *lexeme);
void initLexer(FILE *source, const char *file);
Token getNextToken();


#endif // LEXER_H