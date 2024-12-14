#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "frontend/ast/definitions.h"
#include "frontend/ast/core.h"
#include "frontend/parser/core.h"
#include "frontend/parser/expressions/binary_operations/parse_logical_not_expr.h"
#include "frontend/parser/expressions/parse_unary_expr.h"
#include "utils.h"

AstNode *parse_logical_not_expr(Parser *parser) {
  if (current_token(parser).type == TOKEN_NOT) {
    int line = current_token(parser).line;
    int column_start = current_token(parser).column_start;
    int column_end = current_token(parser).column_end;
    int position_start = current_token(parser).position_start;
    int position_end = current_token(parser).position_end;

    AstNode *operand = parse_logical_not_expr(parser);

    LogicalNotExpr *logical_not_data = MALLOC_S(sizeof(LogicalNotExpr));
    logical_not_data->op = operand;

    column_end = current_token(parser).column_end - 1;
    position_end = current_token(parser).position_end - 1;

    AstNode *logical_not_node = create_ast_node(
      NODE_LOGICAL_NOT,
      logical_not_data,
      line,
      column_start,
      position_start,
      column_end,
      position_end
    );

    return logical_not_node;
  } else {
    return parse_unary_expr(parser);
  }
}
