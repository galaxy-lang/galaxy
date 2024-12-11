#include "frontend/parser/printer/nodes/expressions/print_call.h"
#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/print_indent.h"
#include "frontend/parser/printer/print_ast.h"
#include "frontend/parser/printer/visited.h"

void print_call(const AstNode *node, int depth, VisitedNodes *visited){
  CallNode *call_data = (CallNode *)node->data;
  print_indent(depth + 1);

  printf("Caller:\n");
  print_ast_node(call_data->caller, depth + 2, visited);

  print_indent(depth + 1);
  printf("Arguments:\n");
  if (call_data->args) {
    for (size_t i = 0; i < call_data->arg_count; i++) {
      print_ast_node(call_data->args[i], depth + 2, visited);
    }
  } else {
    print_indent(depth + 2);
    printf("No arguments\n");
  }
}