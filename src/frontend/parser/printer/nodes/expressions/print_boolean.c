#include "frontend/parser/printer/nodes/expressions/print_boolean.h"
#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/print_indent.h"

void print_boolean(const AstNode *node, int depth){
  BooleanLiteralNode *boolean_data = (BooleanLiteralNode *)node->data;
  if (boolean_data) {
    print_indent(depth + 1);
    printf("Value: %s\n", boolean_data->value);
  } else {
    printf("Value: (NULL data)\n");
  }
}