#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../../../include/ast/definitions.h"
#include "../../../include/ast/core.h"
#include "../../../include/lexer/core.h"
#include "../../../include/parser/statements/parse_stmt.h"
#include "../../../include/parser/print_ast.h"
#include "../../../include/parser/core.h"

#define MAX_LINE_LENGTH 1024

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

void read_lines(const char *filename, Parser *parser) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char **lines = NULL;
    char buffer[MAX_LINE_LENGTH];
    int line_count = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        lines = realloc(lines, sizeof(char *) * (line_count + 1));
        if (!lines) {
            fprintf(stderr, "Error: Memory allocation failed for lines\n");
            exit(EXIT_FAILURE);
        }
        lines[line_count] = strdup(buffer);
        if (!lines[line_count]) {
            fprintf(stderr, "Error: Memory allocation failed for line content\n");
            exit(EXIT_FAILURE);
        }
        line_count++;
    }

    fclose(file);

    parser->lines = lines;
    parser->line_count = line_count;
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

void error(Parser *parser, const char *message) {
    Token token = at(parser);
    int line = token.line;
    int column_start = token.column_start;
    int column_end = token.column_end;

    fprintf(stderr, "ERROR:\n");
    fprintf(stderr, "%s:%d:%d: %s\n", token.filename, line, column_start, message);

    if (parser->lines && line - 1 < parser->line_count) {
        char *line_content = parser->lines[line - 1];
        fprintf(stderr, " %d |   %s\n", line, line_content);
        fprintf(stderr, "    ");

        for (int i = 0; i < column_start - 1; i++) {
            fprintf(stderr, " ");
        }
        for (int i = column_start; i < column_end; i++) {
            fprintf(stderr, "^");
        }
        fprintf(stderr, "\n");
    }

    parser->errstate = true;
    eat(parser);
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

AstNode *produce_ast(Parser *parser, Token *tokens, int token_count) {
    parser->tokens = tokens;
    parser->token_count = token_count;
    parser->index = 0;

    read_lines(tokens->filename, parser);

    ProgramNode *program_data = malloc(sizeof(ProgramNode));
    program_data->statements = NULL;
    program_data->statement_count = 0;

    AstNode *program_node = create_ast_node(NODE_PROGRAM, program_data);

    while (not_eof(parser)) {
        AstNode *stmt_node = parse_stmt(parser);

        if (stmt_node) {
            program_data->statements = realloc(
                program_data->statements,
                sizeof(AstNode *) * (program_data->statement_count + 1)
            );

            if (!program_data->statements) {
                fprintf(stderr, "Error: Memory allocation failed for statements\n");
                exit(EXIT_FAILURE);
            }

            program_data->statements[program_data->statement_count] = stmt_node;
            program_data->statement_count++;
        }
    }

    return program_node;
}

