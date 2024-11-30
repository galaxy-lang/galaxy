#include "frontend/parser/printer/nodes/print_pre_decrement.h"
#include "frontend/parser/printer/visited.h"
#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/print_indent.h"
#include "frontend/parser/printer/print_ast.h"

void print_pre_decrement(const AstNode *node, int depth, VisitedNodes *visited) {
  PreDecrementExpr *pre_decrement_data = (PreDecrementExpr *)node->data;

  print_indent(depth + 1);
 
  if (pre_decrement_data) {
    if (pre_decrement_data->op){
      printf("Operand:\n");
      print_ast_node(pre_decrement_data->op, depth + 2, visited);
    } else {
      printf("Operand: <NULL OPERAND>\n");                    
      }
  } else {
    printf("Operand: <NULL>\n");
  }
}
