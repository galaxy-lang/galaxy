#include "frontend/parser/printer/nodes/expressions/print_sizeof.h"
#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/print_indent.h"
#include "frontend/parser/printer/print_ast.h"
#include "frontend/parser/printer/visited.h"

void print_sizeof(const AstNode *node, int depth){
    SizeofNode *sizeof_data = (SizeofNode *)node->data;
  
    print_indent(depth + 1);
    printf("Value: ");
    if (sizeof_data->expression) {
      printf("%s\n", sizeof_data->expression);
    } else {
      print_indent(depth + 2);
      printf("No Value\n");
    }
  }