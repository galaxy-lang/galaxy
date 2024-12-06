#include "frontend/parser/printer/nodes/expressions/print_pre_increment.h"
#include "frontend/parser/printer/visited.h"
#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/print_indent.h"
#include "frontend/parser/printer/print_ast.h"

void print_pre_increment(const AstNode *node, int depth, VisitedNodes *visited) {
  PreIncrementExpr *pre_increment_data = (PreIncrementExpr *)node->data;

  print_indent(depth + 1);
  
  if (pre_increment_data) {
    if (pre_increment_data->op){
      printf("Operand:\n");
      print_ast_node(pre_increment_data->op, depth + 2, visited);
    } else {
      printf("Operand: <NULL OPERAND>\n");                    
      }
  } else {
    printf("Operand: <NULL>\n");
  }
}
