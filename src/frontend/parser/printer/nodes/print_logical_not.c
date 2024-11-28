#include "../../../../../include/frontend/parser/printer/nodes/print_logical_not.h"
#include "../../../../../include/frontend/parser/printer/visited.h"
#include "../../../../../include/frontend/ast/definitions.h"
#include "../../../../../include/frontend/parser/printer/print_indent.h"
#include "../../../../../include/frontend/parser/printer/print_ast.h"

void print_logical_not(const AstNode *node, int depth, VisitedNodes *visited) {
  LogicalNotExpr *logical_not_data = (LogicalNotExpr *)node->data;

  print_indent(depth + 1);

  if (logical_not_data) {
    if (logical_not_data->op){
      printf("Operand:\n");
      print_ast_node(logical_not_data->op, depth + 2, visited);
    } else {
      printf("Operand: <NULL OPERAND>\n");                    
      }
  } else {
    printf("Operand: <NULL>\n");
  }
}
