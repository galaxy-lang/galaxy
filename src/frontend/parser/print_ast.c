#include <stdio.h>
#include "../../../include/ast/definitions.h"

const char* returnASTNodeName(NodeType node_type) {
    switch (node_type) {
        case NODE_PROGRAM: return "Program";
        case NODE_NUMERIC_LITERAL: return "Numeric Literal";
        case NODE_IDENTIFIER: return "Identifier";
        case NODE_BINARY_EXPR: return "Binary Expression";
        default: return "Unknown";
    }
}

void print_ast_node(const AstNode *node, int depth) {
    if (!node) return;

    for (int i = 0; i < depth + 1; i++) {
        printf("  ");
    }

    printf("Node Type: %s\n", returnASTNodeName(node->kind));

    if (node->data) {
        switch (node->kind) {
            case NODE_PROGRAM:
                printf("Value: (Program node does not have data)\n");
                break;

            case NODE_NUMERIC_LITERAL:
                printf("Value: %f\n", ((NumericLiteralNode*)node->data)->value);
                break;

            case NODE_IDENTIFIER:
                printf("Value: %s\n", ((IdentifierNode*)node->data)->symbol);
                break;

            case NODE_BINARY_EXPR:
                {
                    BinaryExprNode *binary_expr = (BinaryExprNode*)node->data;
                    if (binary_expr) {
                        printf("Left Operand: %p\n", binary_expr->left);
                        printf("Right Operand: %p\n", binary_expr->right);
                        printf("Operator: %s\n", binary_expr->operator);
                    } else {
                        printf("BinaryExprNode data is NULL\n");
                    }
                }
                break;

            default:
                printf("Value: Unknown\n");
                break;
        }
    } else {
        printf("Node data is NULL\n");
    }

    if (node->children) {
        for (size_t i = 0; i < node->child_count; i++) {
            print_ast_node(node->children[i], depth + 1);
        }
    } else {
        printf("Node has no children or children pointer is NULL\n");
    }
}

void print_ast(const AstNode *root) {
    printf("AST:\n");
    print_ast_node(root, 0);
}
