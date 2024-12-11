#include "frontend/parser/printer/nodes/expressions/print_ternary.h"
#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/print_indent.h"
#include "frontend/parser/printer/print_ast.h"
#include "frontend/parser/printer/visited.h"

void print_ternary(const AstNode *node, int depth, VisitedNodes *visited){
  TernaryNode *ternary_data = (TernaryNode *)node->data;

  print_indent(depth + 1);
  printf("Condition:\n");
  print_ast_node(ternary_data->condition, depth + 2, visited);
  
  print_indent(depth + 1);
  printf("Consequent:\n");
  print_ast_node(ternary_data->consequent, depth + 2, visited);

  print_indent(depth + 1);
  printf("Alternate:\n");
  print_ast_node(ternary_data->alternate, depth + 2, visited);
}