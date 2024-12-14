#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "frontend/ast/definitions.h"
#include "frontend/ast/core.h"
#include "frontend/parser/core.h"
#include "frontend/parser/expressions/binary_operations/parse_logical_not_expr.h"
#include "frontend/parser/expressions/binary_operations/parse_relational_expr.h"

AstNode *parse_relational_expr(Parser *parser) {
  int line = current_token(parser).line;
  int column_start = current_token(parser).column_start;
  int column_end = current_token(parser).column_end;
  int position_start = current_token(parser).position_start;
  int position_end = current_token(parser).position_end;

  AstNode *left = parse_logical_not_expr(parser);

  while(current_token(parser).type == TOKEN_GT 
    || current_token(parser).type == TOKEN_GEQUAL
    || current_token(parser).type == TOKEN_LT
    || current_token(parser).type == TOKEN_LEQUAL
  ) {
    char *operator = strdup(consume_token(parser).lexeme);

    AstNode *right = parse_logical_not_expr(parser);

    column_end = current_token(parser).column_end - 1;
    position_end = current_token(parser).position_end - 1;

    AstNode *bin_expr = create_ast_node(
      NODE_BINARY_EXPR,
      create_binary_expr_data(left, right, operator),
      line,
      column_start,
      position_start,
      column_end,
      position_end
    );

    add_child_to_node(bin_expr, left);
    add_child_to_node(bin_expr, right);

    left = bin_expr;
  }

  return left;
}
