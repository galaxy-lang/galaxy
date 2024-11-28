#include "../../../../../include/parser/printer/nodes/print_pre_decrement.h"
#include "../../../../../include/parser/printer/visited.h"
#include "../../../../../include/ast/definitions.h"
#include "../../../../../include/parser/printer/print_indent.h"

void print_pre_decrement(const AstNode *node, int depth, VisitedNodes *visited) {
  PreDecrementExpr *pre_decrement_data = (PreDecrementExpr *)node->data;

  if (pre_decrement_data) {
    if (pre_decrement_data->op){
      print_indent(depth + 1);
      printf("Operand:\n");
      print_ast_node(pre_decrement_data->op, depth + 2, visited);
    } else {
      print_indent(depth + 1);
      printf("Operand: <NULL OPERAND>\n");                    
      }
  } else {
    print_indent(depth + 1);
    printf("Operand: <NULL>\n");
  }
}
