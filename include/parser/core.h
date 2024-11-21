#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

typedef struct {
    Token *tokens;
    int token_count;
    bool errstate;
    char **lines;
    int line_count;
    int index;
} Parser;

Parser parser_new();

bool not_eof(Parser *parser);

Token at(Parser *parser);

Token eat(Parser *parser);

Token next(Parser *parser);

Token expect(Parser *parser, TokenType expected_type, const char *err);

void error(Parser *parser, const char *message);

void produce_ast(Parser *parser, Token *tokens, int token_count, const char *source_code, Program *program);

void free_parser(Parser *parser);

#endif // PARSER_H
