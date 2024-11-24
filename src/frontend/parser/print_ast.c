#include <stdio.h>
#include "../../../include/ast/definitions.h"

/**
 * @brief Returns the textual name of an AST node type.
 *
 * This function returns a string representing the name of the node type based on the 
 * enumerated value `NodeType`. It is useful for printing information about AST nodes.
 *
 * @param node_type The type of the AST node (enum `NodeType`).
 * @return A string with the name of the node type. Returns "Unknown" if the type is not recognized.
 */
const char* returnASTNodeName(NodeType node_type) {
    switch (node_type) {
        case NODE_PROGRAM: return "Program";
        case NODE_NUMERIC_LITERAL: return "Numeric Literal";
        case NODE_IDENTIFIER: return "Identifier";
        case NODE_BINARY_EXPR: return "Binary Expression";
        default: return "Unknown";
    }
}

/**
 * @brief Prints the indentation corresponding to the depth of a node.
 *
 * This function inserts spaces to create an indentation effect proportional to the depth 
 * specified. Each depth level adds two spaces.
 *
 * @param depth The depth of the node in the AST.
 */
void print_indent(int depth) {
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
}

/**
 * @brief Prints an AST node and its details.
 *
 * This function displays information about a specific AST node, including its type and 
 * associated data. Depending on the type, it prints literal values, operators, or child 
 * nodes associated with the node. It uses indentation to reflect the node's depth in the AST structure.
 *
 * @param node The AST node to be printed.
 * @param depth The depth of the node in the AST, used for visual indentation.
 */
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

    if (node->children) {
        for (size_t i = 0; i < node->child_count; i++) {
            if (node->children[i]) {
                print_ast_node(node->children[i], depth + 1);
            } else {
                printf("Child %zu is NULL\n", i);
            }
        }
    }
}

void print_ast(const AstNode *root) {
    printf("AST:\n");
    print_ast_node(root, 0);
}
