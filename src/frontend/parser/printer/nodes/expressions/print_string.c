#include "frontend/parser/printer/nodes/expressions/print_string.h"
#include "frontend/parser/printer/visited.h"
#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/print_indent.h"
#include "frontend/parser/printer/print_ast.h"

void print_string(const AstNode *node, int depth) {
  StringNode *string_data = (StringNode *)node->data;
  print_indent(depth + 1);
    
  if (string_data->string) {
    printf("String: %s\n", string_data->string);
  } else {
    printf("String: <NULL>\n");
  }
}