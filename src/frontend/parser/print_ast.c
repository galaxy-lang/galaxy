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

void print_indent(int depth) {
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
}

void print_ast_node(const AstNode *node, int depth) {
    if (!node) return;

    print_indent(depth);
    printf("Node Type: %s\n", returnASTNodeName(node->kind));

    switch (node->kind) {
        case NODE_PROGRAM: {
            print_indent(depth + 1);
            ProgramNode *program_data = (ProgramNode *)node->data;
            if (program_data && program_data->statements) {
                printf("Program has %zu statements\n", program_data->statement_count);
                for (size_t i = 0; i < program_data->statement_count; i++) {
                    print_ast_node(program_data->statements[i], depth + 1);
                }
            } else {
                printf("Value: (No statements or NULL data)\n");
            }
            break;
        }

        case NODE_NUMERIC_LITERAL: {
            NumericLiteralNode *literal_data = (NumericLiteralNode *)node->data;
            if (literal_data) {
                print_indent(depth + 1);
                printf("Value: %f\n", literal_data->value);
            } else {
                printf("Value: (NULL data)\n");
            }
            break;
        }

        case NODE_IDENTIFIER: {
            IdentifierNode *identifier_data = (IdentifierNode *)node->data;
            if (identifier_data) {
                print_indent(depth + 1);
                printf("Value: %s\n", identifier_data->symbol);
            } else {
                printf("Value: (NULL data)\n");
            }
            break;
        }

        case NODE_BINARY_EXPR: {
            BinaryExprNode *binary_expr_data = (BinaryExprNode *)node->data;
            if (binary_expr_data) {
                print_indent(depth + 1);
                printf("Operator: %s\n", binary_expr_data->operator);

                print_indent(depth + 1);
                printf("Left Operand:\n");
                print_ast_node(binary_expr_data->left, depth + 2);

                print_indent(depth + 1);
                printf("Right Operand:\n");
                print_ast_node(binary_expr_data->right, depth + 2);
            } else {
                printf("Value: (NULL data)\n");
            }
            break;
        }

        default:
            print_indent(depth + 1);
            printf("Value: Unknown or no data\n");
            break;
    }

    for (size_t i = 0; i < node->child_count; i++) {
        print_ast_node(node->children[i], depth + 1);
    }
}

void print_ast(const AstNode *root) {
    printf("AST:\n");
    print_ast_node(root, 0);
}
