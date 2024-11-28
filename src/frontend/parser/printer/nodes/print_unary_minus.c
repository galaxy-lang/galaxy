#include "../../../../../include/parser/printer/nodes/print_unary_minus.h"
#include "../../../../../include/parser/printer/visited.h"
#include "../../../../../include/ast/definitions.h"
#include "../../../../../include/parser/printer/print_indent.h"

void print_unary_minus(const AstNode* node, int depth, VisitedNodes* visited) {
    UnaryMinusExpr* unexpr_data = (UnaryMinusExpr*)node->data;
    print_indent(depth + 1);
    if (unexpr_data) {
        if (unexpr_data->op) {
            printf("Negation:\n");
            print_ast_node(unexpr_data->op, depth + 2, visited);
        } else {
            printf("Negation: <NULL OPERAND>\n");
        }
    } else {
        printf("Negation: <NULL>\n");
    }
}