#ifndef PARSER_H
#define PARSER_H

#include "../ast/core.h"
#include "../ast/definitions.h"
#include "../lexer/core.h"

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
Token current_token(Parser *parser);
Token consume_token(Parser *parser);
Token next_token(Parser *parser);
Token expect(Parser *parser, TokenType expected_type, const char *err);
void error(Parser *parser, const char *message);
AstNode *produce_ast(Parser *parser, Token *tokens, int token_count);
void read_lines(const char *filename, Parser *parser);

#endif // PARSER_H
