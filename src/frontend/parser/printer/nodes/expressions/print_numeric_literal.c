#include "frontend/parser/printer/nodes/expressions/print_numeric_literal.h"
#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/print_indent.h"

void print_numeric_literal(const AstNode *node, int depth) {
  NumericLiteralNode *literal_data = (NumericLiteralNode *)node->data;
  
  if (literal_data) {
    print_indent(depth + 1);
    printf("Value: %f\n", literal_data->value);
  } else {
    printf("Value: (NULL data)\n");
  }
}