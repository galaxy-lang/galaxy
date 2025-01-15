#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "frontend/parser/statements/parse_while_stmt.h"
#include "frontend/parser/statements/parse_stmt.h"
#include "frontend/parser/expressions/parse_expr.h"
#include "frontend/parser/expressions/binary_operations/parse_logical_expr.h"
#include "frontend/parser/statements/parse_variable_declaration_stmt.h"
#include "frontend/parser/types/parse_type.h"
#include "frontend/parser/core.h"
#include "utils.h"


AstNode *parse_while_stmt(Parser *parser) {
    int line = current_token(parser).line;
    int column_start = current_token(parser).column_start;
    int position_start = current_token(parser).position_start;

    consume_token(parser);
    expect(parser, TOKEN_OPAREN, "Expected \"(\".");
    AstNode *condition = parse_logical_expr(parser);

    expect(parser, TOKEN_CPAREN, "Expected \")\".");
    expect(parser, TOKEN_COLON, "Expected \":\".");

    WhileNode *while_data = MALLOC_S(sizeof(WhileNode));
    while_data->body = NULL;
    while_data->body_count = 0;

    if (not_eof(parser) && current_token(parser).type != TOKEN_END) {
        while_data->body = REALLOC_S(
            while_data->body,
            sizeof(AstNode *) * (while_data->body_count + 1)
        );

        while_data->body[while_data->body_count++] = parse_stmt(parser);
    }

    expect(parser, TOKEN_END, "Expected \"end\" to close while statement.");

    int column_end = current_token(parser).column_end - 1;
    int position_end = current_token(parser).position_end - 1;

    expect(parser, TOKEN_SEMICOLON, "Expected \";\".");

    AstNode *while_node = create_ast_node(
        NODE_WHILE,
        while_data,
        line,
        column_start,
        position_start,
        column_end,
        position_end
    );

    return while_node;
}