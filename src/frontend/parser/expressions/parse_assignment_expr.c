#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../../../../include/ast/definitions.h"
#include "../../../../include/parser/expressions/parse_assignment_expr.h"
#include "../../../../include/parser/expressions/binary_operations/parse_additive_expr.h"

AstNode *parse_assignment_expr(Parser *parser) {
  int line = at(parser).line;
  int column_start = at(parser).column_start;
  int position_start = at(parser).position_start;

  AstNode *left = parse_additive_expr(parser);

  if (at(parser).type == TOKEN_ASSIGN) {
    eat(parser);

    AstNode *value = parse_assignment_expr(parser);

    int column_end = at(parser).column_end - 1;
    int position_end = at(parser).position_end - 1;

    AssignmentNode *assignment_data = malloc(sizeof(AssignmentNode));
    if (!assignment_data) {
      fprintf(stderr, "Erro: Falha ao alocar memória para AssignmentNode\n");
      exit(EXIT_FAILURE);
    }
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
