#include "../../../../../include/parser/printer/nodes/print_binary_expr.h"
#include "../../../../../include/parser/printer/visited.h"
#include "../../../../../include/ast/definitions.h"

void print_binary_expr(const AstNode *node, int depth, VisitedNodes *visited) {
  BinaryExprNode *binary_expr_data = (BinaryExprNode *)node->data;
            
  if (binary_expr_data) {
    print_indent(depth + 1);
    printf("Operator: %s\n", binary_expr_data->operator);

    print_indent(depth + 1);
    printf("Left Operand:\n");
    print_ast_node(binary_expr_data->left, depth + 2, visited);

    print_indent(depth + 1);
    printf("Right Operand:\n");
    print_ast_node(binary_expr_data->right, depth + 2, visited);
  } else {
    printf("Value: (NULL data)\n");
  }
}