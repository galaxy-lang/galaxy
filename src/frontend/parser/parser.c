#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../../../include/ast.h"

typedef struct {
    Token *tokens;
    int token_count;
    bool errstate;
    char **lines;
    int line_count;
    int index;
} Parser;

Parser parser_new() {
    Parser parser;
    parser.tokens = NULL;
    parser.token_count = 0;
    parser.errstate = false;
    parser.lines = NULL;
    parser.line_count = 0;
    parser.index = 0;
    return parser;
}

bool not_eof(Parser *parser) {
    return parser->tokens[parser->index].type != TOKEN_EOF;
}

Token at(Parser *parser) {
    return parser->tokens[parser->index];
}

Token eat(Parser *parser) {
    return parser->tokens[parser->index++];
}

Token next(Parser *parser) {
    if (parser->index + 1 >= parser->token_count) {
        return at(parser);
    }
    return parser->tokens[parser->index + 1];
}

Token expect(Parser *parser, TokenType expected_type, const char *err) {
    Token prev = eat(parser);

    if (prev.type == TOKEN_EOF) {
        return prev;
    }

    if (prev.type != expected_type) {
        if (prev.type == TOKEN_UNKNOWN) {
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg), "Expected '%d' token, but got a bad token: '%s'", expected_type, prev.lexeme);
            error(parser, error_msg);
        } else {
            error(parser, err);
        }
    }

    return prev;
}

void error(Parser *parser, const char *message) {
    Token token = at(parser);
    int line = token.line;
    int column_start = token.column_start;
    int column_end = token.column_end;

    fprintf(stderr, "ERROR: %s\n", message);
    fprintf(stderr, "%s:%d:%d\n", token.filename, line, column_start);

    if (parser->lines && line - 1 < parser->line_count) {
        char *line_content = parser->lines[line - 1];
        fprintf(stderr, "    %s\n", line_content);
        for (int i = 0; i < column_start - 1; i++) {
            fprintf(stderr, " ");
        }
        for (int i = column_start; i <= column_end; i++) {
            fprintf(stderr, "^");
        }
        fprintf(stderr, "\n");
    }

    parser->errstate = true;
    eat(parser);
}

void produce_ast(Parser *parser, Token *tokens, int token_count, const char *source_code, Program *program) {
    parser->tokens = tokens;
    parser->token_count = token_count;
    parser->lines = malloc(sizeof(char *) * token_count);
    parser->line_count = 0;

    char *source_copy = strdup(source_code);
    char *line = strtok(source_copy, "\n");

    while (line) {
        parser->lines[parser->line_count++] = strdup(line);
        line = strtok(NULL, "\n");
    }

    free(source_copy);

    program_init(program);

    while (at(parser).type != TOKEN_EOF) {
        AstNode *stmt_node = ast_node_new(NODE_STATEMENT, NULL);
        add_node_item(program, stmt_node);
    }
}

void free_parser(Parser *parser) {
    if (parser->lines) {
        for (int i = 0; i < parser->line_count; i++) {
            free(parser->lines[i]);
        }
        free(parser->lines);
    }
    parser->lines = NULL;
    parser->line_count = 0;
}
