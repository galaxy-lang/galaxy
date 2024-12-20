#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "frontend/parser/statements/parse_for_stmt.h"
#include "frontend/parser/statements/parse_stmt.h"
#include "frontend/parser/expressions/parse_expr.h"
#include "frontend/parser/expressions/binary_operations/parse_logical_expr.h"
#include "frontend/parser/statements/parse_variable_declaration_stmt.h"
#include "frontend/parser/types/parse_type.h"
#include "frontend/parser/core.h"
#include "utils.h"

AstNode *parse_for_stmt(Parser *parser) {
    int line = current_token(parser).line;
    int column_start = current_token(parser).column_start;
    int position_start = current_token(parser).position_start;

    consume_token(parser);
    bool var_isPtr = false, 
         is_parallel = false;

    char *var_type = "int",
         *variable = NULL,
         *schedule_policy = NULL;

    AstNode *iterator = NULL,
            *start = NULL,
            *stop = NULL,
            *updater = NULL,
            *num_threads = NULL;

    if (current_token(parser).type == TOKEN_PARALLEL) {
        consume_token(parser);
        is_parallel = true;

        if (current_token(parser).type != TOKEN_STATIC && current_token(parser).type != TOKEN_DYNAMIC) {
            error(parser, "Expected schedule policy");
            exit(EXIT_FAILURE);
        }

        schedule_policy = consume_token(parser).lexeme;
    }
    

    expect(parser, TOKEN_OPAREN, "Expected \"(\".");


    if (current_token(parser).type != TOKEN_IDENTIFIER) {
        var_type = parse_type(parser);
        if (current_token(parser).type == TOKEN_MUL) {
            var_isPtr = true;
            consume_token(parser);
        }
    }

    variable = expect(parser, TOKEN_IDENTIFIER, "Expected variable identifier.").lexeme;

    if (current_token(parser).type == TOKEN_ASSIGN) {
        consume_token(parser);
        printf("Token: %s\n", current_token(parser).lexeme);
        start = parse_expr(parser);

        expect(parser, TOKEN_SEMICOLON, "Expected \";\".");

        printf("Token: %s\n", current_token(parser).lexeme);
        stop = parse_logical_expr(parser);

        expect(parser, TOKEN_SEMICOLON, "Expected \";\".");

        updater = parse_expr(parser);

    } else if (current_token(parser).type == TOKEN_COLON) {
        consume_token(parser);
        if (next_token(parser).type == TOKEN_RANGE || next_token(parser).type == TOKEN_IRANGE) {
            start = parse_expr(parser);
            consume_token(parser);
            stop = parse_expr(parser);
        } else {
            iterator = parse_expr(parser);
        }

    } else {
        error(parser, "Unexpected token in for statement.");
        exit(EXIT_FAILURE);
    }

    expect(parser, TOKEN_CPAREN, "Expected \")\".");

    if (current_token(parser).type == TOKEN_ARROW) {
        consume_token(parser);
        num_threads = parse_expr(parser);
    }

    expect(parser, TOKEN_COLON, "Expected \":\".");

    ForNode *for_data = MALLOC_S(sizeof(ForNode));
    for_data->body = NULL;
    for_data->body_count = 0;
    for_data->variable = variable;
    for_data->var_type = var_type;
    for_data->var_isPtr = var_isPtr;
    for_data->start = start;
    for_data->stop = stop;
    for_data->updater = updater;
    for_data->iterator = iterator;
    for_data->is_parallel = is_parallel;
    for_data->schedule_policy = schedule_policy;
    for_data->num_threads = num_threads;

    while (not_eof(parser) && current_token(parser).type != TOKEN_END) {
        for_data->body = REALLOC_S(
            for_data->body,
            sizeof(AstNode *) * (for_data->body_count + 1)
        );

        for_data->body[for_data->body_count++] = parse_stmt(parser);
    }

    expect(parser, TOKEN_END, "Expected \"end\" to close for statement.");

    int column_end = current_token(parser).column_end - 1;
    int position_end = current_token(parser).position_end - 1;

    expect(parser, TOKEN_SEMICOLON, "Expected \";\".");
    AstNode *for_node = create_ast_node(
        NODE_FOR,
        for_data,
        line,
        column_start,
        position_start,
        column_end,
        position_end
    );

    return for_node;
}
