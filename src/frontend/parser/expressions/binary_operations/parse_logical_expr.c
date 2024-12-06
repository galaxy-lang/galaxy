#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "frontend/ast/definitions.h"
#include "frontend/ast/core.h"
#include "frontend/parser/core.h"
#include "frontend/parser/expressions/binary_operations/parse_equality_expr.h"
#include "frontend/parser/expressions/binary_operations/parse_logical_expr.h"

AstNode *parse_logical_expr(Parser *parser) {
  int line = at(parser).line;
  int column_start = at(parser).column_start;
  int column_end = at(parser).column_end;
  int position_start = at(parser).position_start;
  int position_end = at(parser).position_end;

  AstNode *left = parse_equality_expr(parser);

  while(at(parser).type == TOKEN_AND || at(parser).type == TOKEN_OR) {
    char *operator = strdup(at(parser).lexeme);
    eat(parser);

    AstNode *right = parse_equality_expr(parser);

    column_end = at(parser).column_end - 1;
    position_end = at(parser).position_end - 1;

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
