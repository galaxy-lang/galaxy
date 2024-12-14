#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "frontend/lexer/core.h"
#include "frontend/parser/core.h"
#include "frontend/parser/expressions/parse_ternary_expr.h"
#include "frontend/parser/expressions/parse_nested_ternary_expr.h"
#include "frontend/parser/expressions/parse_array_access_expr.h"
#include "frontend/parser/expressions/parse_call_member_expr.h"
#include "frontend/parser/expressions/binary_operations/parse_logical_expr.h"

AstNode *parse_ternary_expr(Parser *parser) {
  if (
    current_token(parser).type == TOKEN_IDENTIFIER
    && next_token(parser).type == TOKEN_OBRACKET
  ) {
    AstNode *identifier = parse_call_member_expr(parser, NULL);
    return parse_array_access_expr(parser, identifier);
  }

  int line = current_token(parser).line;
  int column_start = current_token(parser).column_start;
  int position_start = current_token(parser).position_start;

  AstNode *condition = parse_logical_expr(parser);

  if (current_token(parser).type != TOKEN_QUESTION_MARK) {
    return condition;
  }

  consume_token(parser);

  AstNode *consequent = parse_nested_ternary_expr(parser);

  if (current_token(parser).type != TOKEN_COLON) {
    error(parser, "Expected \":\" after condition's consequent.");
    exit(EXIT_FAILURE);
  }

  consume_token(parser);

  AstNode *alternate = parse_nested_ternary_expr(parser);

  int column_end = current_token(parser).column_end;
  int position_end = current_token(parser).position_end;

  TernaryNode *ternary_data = MALLOC_S(sizeof(TernaryNode));
  ternary_data->condition = condition;
  ternary_data->consequent = consequent;
  ternary_data->alternate = alternate;

  AstNode *ternary_node = create_ast_node(
    NODE_TERNARY,
    ternary_data,
    line,
    column_start,
    position_start,
    column_end,
    position_end
  );

  return ternary_node;
}