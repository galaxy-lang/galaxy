#include <stdio.h>
#include "../../../include/ast/definitions.h"

void print_ast_node(const AstNode *node, int depth) {
    if (!node) return;

    // Indentação para visualização hierárquica
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    // Imprime informações básicas do nó
    printf("Node Type: %d\n", node->type);

    // Imprime o valor do nó, se existir
    if (node->value) {
        for (int i = 0; i < depth + 1; i++) {
            printf("  ");
        }
        printf("Value: %s\n", node->value);
    }

    // Itera pelos filhos
    for (size_t i = 0; i < node->children_count; i++) {
        print_ast_node(node->children[i], depth + 1);
    }
}

void print_ast(const AstNode *root) {
    printf("AST:\n");
    print_ast_node(root, 0);
}

