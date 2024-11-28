#include "../../../../../include/parser/printer/nodes/print_identifier.h"
#include "../../../../../include/ast/definitions.h"

void print_identifier(const AstNode *node, int depth){
  IdentifierNode *identifier_data = (IdentifierNode *)node->data;
  if (identifier_data) {
    print_indent(depth + 1);
    printf("Value: %s\n", identifier_data->symbol);
  } else {
    printf("Value: (NULL data)\n");
  }
}