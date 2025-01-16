#include "frontend/parser/printer/nodes/statements/print_while.h"

void print_while(const AstNode *node, int depth, VisitedNodes *visited) {
    if (!node || node->kind != NODE_WHILE) return;

    WhileNode *while_data = (WhileNode *)node->data;

    print_indent(depth + 1);
    printf("Condition: \n");
    print_ast_node(while_data->condition, depth + 2, visited);

    if (while_data->body_count > 0) {
        print_indent(depth + 1);
        printf("Body: \n");

        for (size_t i = 0; i < while_data->body_count; i++) {
            print_ast_node(while_data->body[i], depth + 2, visited);
        }
    } else {
        print_indent(depth + 1);
        printf("Body: <NULL>\n");
    }
}