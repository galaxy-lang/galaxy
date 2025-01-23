#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "utils.h"
#include "frontend/ast/definitions.h"
#include "frontend/parser/expressions/parse_assignment_expr.h"
#include "frontend/parser/expressions/parse_expr.h"
#include "frontend/parser/expressions/parse_ternary_expr.h"
#include "frontend/parser/expressions/parse_object_expr.h"

AstNode *parse_assignment_expr(Parser *parser) {
  int line = current_token(parser).line;
  int column_start = current_token(parser).column_start;
  int position_start = current_token(parser).position_start;

  AstNode *left = parse_ternary_expr(parser);

  if (current_token(parser).type == TOKEN_ASSIGN) {
    consume_token(parser);

    AstNode *value;

    switch (current_token(parser).type) {
      case TOKEN_OBRACE: value = parse_object_expr(parser);
      case TOKEN_CBRACKET: value = parse_object_expr(parser);
      default: value = parse_ternary_expr(parser);
    }

    int column_end = current_token(parser).column_end - 1;
    int position_end = current_token(parser).position_end - 1;

    expect(parser, TOKEN_SEMICOLON, "Expected \";\".");

    AssignmentNode *assignment_data = MALLOC_S(sizeof(AssignmentNode));
    assignment_data->left = left;
    assignment_data->value = value;

    AstNode *assignment_node = create_ast_node(
      NODE_ASSIGNMENT,
      assignment_data,
      line,
      column_start,
      position_start,
      column_end,
      position_end
    ); 

    return assignment_node;
  }

  return left;
}
