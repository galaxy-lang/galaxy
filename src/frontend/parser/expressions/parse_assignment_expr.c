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
  int line = at(parser).line;
  int column_start = at(parser).column_start;
  int position_start = at(parser).position_start;

  AstNode *left = parse_ternary_expr(parser);

  if (at(parser).type == TOKEN_ASSIGN) {
    eat(parser);

    AstNode *value;

    switch (at(parser).type) {
      case TOKEN_OBRACE: value = parse_object_expr(parser);
      default: value = parse_ternary_expr(parser);
    }

    int column_end = at(parser).column_end - 1;
    int position_end = at(parser).position_end - 1;

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
