#include "../../../../../include/parser/printer/nodes/print_pre_increment.h"
#include "../../../../../include/parser/printer/visited.h"
#include "../../../../../include/ast/definitions.h"
#include "../../../../../include/parser/printer/print_indent.h"


void print_pre_increment(const AstNode *node, int depth, VisitedNodes *visited) {
  PreIncrementExpr *pre_increment_data = (PreIncrementExpr *)node->data;

  if (pre_increment_data) {
    if (pre_increment_data->op){
      print_indent(depth + 1);
      printf("Operand:\n");
      print_ast_node(pre_increment_data->op, depth + 2, visited);
    } else {
      print_indent(depth + 1);
      printf("Operand: <NULL OPERAND>\n");                    
      }
  } else {
    print_indent(depth + 1);
    printf("Operand: <NULL>\n");
  }
}
