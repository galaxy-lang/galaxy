#include "frontend/parser/printer/nodes/expressions/print_return.h"
#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/print_indent.h"
#include "frontend/parser/printer/print_ast.h"
#include "frontend/parser/printer/visited.h"

void print_return(const AstNode *node, int depth, VisitedNodes *visited){
  ReturnNode *return_data = (ReturnNode *)node->data;

  print_indent(depth + 1);
  printf("Value:\n");
  if (return_data->value) {
    print_ast_node(return_data->value, depth + 2, visited);
  } else {
    print_indent(depth + 2);
    printf("No Value\n");
  }
}