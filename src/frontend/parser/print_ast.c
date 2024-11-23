#include <stdio.h>
#include "../../../include/ast/definitions.h"

void print_ast_node(const AstNode *node, int depth) {
    if (!node) return;

    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    printf("Node Type: %d\n", node->kind);

    if (node->data) {
        for (int i = 0; i < depth + 1; i++) {
            printf("  ");
        }
        printf("Value: %s\n", node->data);
    }

    for (size_t i = 0; i < node->child_count; i++) {
        print_ast_node(node->children[i], depth + 1);
    }
}

void print_ast(const AstNode *root) {
    printf("AST:\n");
    print_ast_node(root, 0);
}

