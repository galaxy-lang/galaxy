#include "../../../../../include/parser/printer/nodes/print_logical_not.h"
#include "../../../../../include/parser/printer/visited.h"
#include "../../../../../include/ast/definitions.h"
#include "../../../../../include/parser/printer/print_indent.h"

void print_logical_not(const AstNode *node, int depth, VisitedNodes *visited) {
  LogicalNotExpr *logical_not_data = (LogicalNotExpr *)node->data;

  if (logical_not_data) {
    if (logical_not_data->op){
      print_indent(depth + 1);
      printf("Operand:\n");
      print_ast_node(logical_not_data->op, depth + 2, visited);
    } else {
      print_indent(depth + 1);
      printf("Operand: <NULL OPERAND>\n");                    
      }
  } else {
    print_indent(depth + 1);
    printf("Operand: <NULL>\n");
  }
}
