#include "frontend/parser/printer/nodes/expressions/print_binary_expr.h"
#include "frontend/parser/printer/visited.h"
#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/print_indent.h"
#include "frontend/parser/printer/print_ast.h"

void print_binary_expr(const AstNode *node, int depth, VisitedNodes *visited) {
  BinaryExprNode *binary_expr_data = (BinaryExprNode *)node->data;

  if (binary_expr_data) {
    print_indent(depth + 1);
    printf("Operator: %s\n", binary_expr_data->op);

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
