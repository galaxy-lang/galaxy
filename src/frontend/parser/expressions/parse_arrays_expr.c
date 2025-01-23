#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "frontend/parser/expressions/parse_arrays_expr.h"
#include "frontend/parser/expressions/parse_expr.h"
#include "frontend/parser/expressions/parse_object_expr.h"
#include "frontend/parser/expressions/binary_operations/parse_logical_expr.h"
#include "frontend/parser/statements/parse_variable_declaration_stmt.h"
#include "frontend/parser/types/parse_type.h"
#include "frontend/parser/core.h"
#include "utils.h"

AstNode *parse_arrays_expr(Parser *parser) {
    int line = current_token(parser).line;
    int column_start = current_token(parser).column_start;
    int position_start = current_token(parser).position_start;

    if (current_token(parser).type != TOKEN_OBRACKET) {
        parse_object_expr(parser);
    }

    consume_token(parser);

    ArrayNode *array_data = MALLOC_S(sizeof(ArrayNode));

    array_data->elements = NULL;
    array_data->element_count = 0;

    while (current_token(parser).type != TOKEN_CBRACKET) {
        array_data->elements = REALLOC_S(
            array_data->elements,
            sizeof(AstNode *) * array_data->element_count + 1);
    

        array_data->elements[array_data->element_count++] = parse_arrays_expr(parser);

        if (current_token(parser).type == TOKEN_COMMA) {
            consume_token(parser);
        }
    }

    expect(parser, TOKEN_CBRACKET, "\"]\" Expected.");

    int column_end = current_token(parser).column_end;
    int position_end = current_token(parser).position_end;

    AstNode *array_node = create_ast_node(
        NODE_ARRAY,
        array_data,
        line,
        column_start,
        position_start,
        column_end,
        position_end
    );
}