#include "../../../../../include/parser/printer/nodes/print_numeric_literal.h"
#include "../../../../../include/ast/definitions.h"

void print_numeric_literal(const AstNode *node, int depth) {
  NumericLiteralNode *literal_data = (NumericLiteralNode *)node->data;
  
  if (literal_data) {
    print_indent(depth + 1);
    printf("Value: %f\n", literal_data->value);
  } else {
    printf("Value: (NULL data)\n");
  }
}