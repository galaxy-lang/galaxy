#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "frontend/lexer/core.h"
#include "frontend/parser/core.h"
#include "frontend/parser/expressions/binary_operations/parse_bitwise_expr.h"
#include "frontend/parser/expressions/binary_operations/parse_additive_expr.h"
#include "frontend/parser/expressions/binary_operations/parse_multiplicative_expr.h"


AstNode *parse_bitwise_expr(Parser *parser)
{
  int line = at(parser).line;
  int column_start = at(parser).column_start;
  int column_end = at(parser).column_end;
  int position_start = at(parser).position_start;
  int position_end = at(parser).position_end;
  AstNode *left = parse_additive_expr(parser);

  while (
      at(parser).type == TOKEN_BITWISE_XOR
      || at(parser).type == TOKEN_BITWISE_NOT
      || at(parser).type == TOKEN_BITWISE_AND
      || at(parser).type == TOKEN_BITWISE_OR
      || at(parser).type == TOKEN_SHIFT_LEFT
      || at(parser).type == TOKEN_SHIFT_RIGHT
  ) {
    char *operator= strdup(at(parser).lexeme);
    eat(parser);

    AstNode *right = parse_additive_expr(parser);

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
