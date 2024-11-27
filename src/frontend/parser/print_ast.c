#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../../include/ast/definitions.h"

/**
 * @brief Auxiliary structure to track visited nodes.
 *
 * This structure keeps a record of the nodes already visited during AST printing,
 * preventing duplication. It uses a dynamic list to store pointers to processed nodes.
 */
typedef struct VisitedNodes {
    const AstNode **nodes; /**< List of pointers to visited nodes. */
    size_t count;          /**< Number of nodes currently stored. */
    size_t capacity;       /**< Maximum capacity of the list before resizing. */
} VisitedNodes;

/**
 * @brief Initializes the VisitedNodes structure.
 *
 * This function allocates memory for the structure and sets its initial capacity.
 *
 * @param visited Pointer to the VisitedNodes structure to initialize.
 */
void init_visited(VisitedNodes *visited) {
    visited->nodes = malloc(sizeof(AstNode *) * 64);
    visited->count = 0;
    visited->capacity = 64;
}

/**
 * @brief Frees the memory used by the VisitedNodes structure.
 *
 * This function releases all resources associated with the VisitedNodes structure.
 *
 * @param visited Pointer to the VisitedNodes structure to free.
 */
void free_visited(VisitedNodes *visited) {
    free(visited->nodes);
}

/**
 * @brief Checks if a node has already been visited.
 *
 * This function determines whether a specific AST node has already been processed.
 *
 * @param visited Pointer to the VisitedNodes structure.
 * @param node Pointer to the AST node to check.
 * @return True if the node has been visited; otherwise, false.
 */
bool is_visited(VisitedNodes *visited, const AstNode *node) {
    for (size_t i = 0; i < visited->count; i++) {
        if (visited->nodes[i] == node) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Marks a node as visited.
 *
 * This function adds a node to the list of visited nodes, resizing the list if needed.
 *
 * @param visited Pointer to the VisitedNodes structure.
 * @param node Pointer to the AST node to mark as visited.
 */
void mark_visited(VisitedNodes *visited, const AstNode *node) {
    if (visited->count == visited->capacity) {
        visited->capacity *= 2;
        visited->nodes = realloc(visited->nodes, sizeof(AstNode *) * visited->capacity);
    }
    visited->nodes[visited->count++] = node;
}

/**
 * @brief Returns the textual name of an AST node type.
 *
 * This function returns a string representing the name of the node type based on the 
 * enumerated value NodeType. It is useful for printing information about AST nodes.
 *
 * @param node_type The type of the AST node (enum NodeType).
 * @return A string with the name of the node type. Returns "Unknown" if the type is not recognized.
 */
const char* returnASTNodeName(NodeType node_type) {
    switch (node_type) {
        case NODE_PROGRAM: return "Program";
        case NODE_NUMERIC_LITERAL: return "Numeric Literal";
        case NODE_IDENTIFIER: return "Identifier";
        case NODE_BINARY_EXPR: return "Binary Expression";
        case NODE_PACKAGE: return "Package Statement";
        case NODE_IMPORT: return "Import Statement";
        case NODE_ASSIGNMENT: return "Assignment Expression";
        case NODE_OBJECT: return "Object Expression";
        case NODE_PROPERTY: return "Property";
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
 * This function displays information about a specific AST node, including its type
 * and associated data. It uses indentation to reflect the node's depth in the AST structure.
 *
 * @param node The AST node to print.
 * @param depth The depth of the node in the AST, used for visual indentation.
 * @param visited Pointer to the VisitedNodes structure to track visited nodes.
 */
void print_ast_node(const AstNode *node, int depth, VisitedNodes *visited) {
    if (!node || is_visited(visited, node)) return;

    mark_visited(visited, node);
    print_indent(depth);
    printf("Node Type: %s\n", returnASTNodeName(node->kind));

    switch (node->kind) {
        case NODE_PROGRAM: {
            print_indent(depth + 1);
            ProgramNode *program_data = (ProgramNode *)node->data;
            if (program_data && program_data->statements) {
                printf("Program has %zu statements\n", program_data->statement_count);
                for (size_t i = 0; i < program_data->statement_count; i++) {
                    print_ast_node(program_data->statements[i], depth + 1, visited);
                }
            } else {
                printf("Value: (No statements or NULL data)\n");
            }
            break;
        }

        case NODE_ASSIGNMENT: {
            AssignmentNode *assignment_data = (AssignmentNode *)node->data;
            if (assignment_data) {
                print_indent(depth + 1);
                printf("Left:\n");
                if (assignment_data->left) {
                    print_ast_node(assignment_data->left, depth + 2, visited);
                } else {
                    print_indent(depth + 2);
                    printf("Left is NULL\n");
                }

                print_indent(depth + 1);
                printf("Value:\n");
                if (assignment_data->value) {
                    print_ast_node(assignment_data->value, depth + 2, visited);
                } else {
                    print_indent(depth + 2);
                    printf("Value is NULL\n");
                }
            } else {
                print_indent(depth + 1);
                printf("Assignment data is NULL\n");
            }
            break;
        }

        case NODE_OBJECT: {
            ObjectNode *object_data = (ObjectNode *)node->data;
            
            if (object_data && object_data->property_count > 0) {
                for (int i = 0; i < object_data->property_count; i++) {
                    PropertyNode *property = (PropertyNode *)object_data->properties[i]->data;

                    if (property) {
                        print_indent(depth + 1);
                        printf("Property: %s\n", property->key);

                        if (property->value){
                            print_ast_node(property->value, depth + 2, visited);
                        }
                    } 
                }
            }
            break;
        }

        case NODE_PROPERTY: {
            PropertyNode *property = (PropertyNode *)node->data;
            if (property) {
                print_indent(depth + 1);
                
                if (property->key) {
                    printf("Key: %s\n", property->key);
                } else {
                    printf("Key: <NULL>\n");
                }

                if (property->value) {
                    print_ast_node(property->value, depth + 2, visited);
                } else {
                    print_indent(depth + 2);
                    printf("Value: <NULL>\n");
                }

            } else {
                print_indent(depth + 1);
                printf("Property: <NULL>\n");
            }
            break;
        }

        case NODE_PACKAGE: {
            PackageNode *package_data = (PackageNode *)node->data;
            if (package_data) {
                print_indent(depth + 1);
                printf("Package Name: %s\n", package_data->package);
            }
            break;
        }


        case NODE_IMPORT: {
            ImportNode *import_data = (ImportNode *)node->data;
            if (import_data && import_data->package_count > 0) {
                for (int i = 0; i < import_data->package_count; i++) {
                    print_indent(depth + 1);
                    printf("Package: %s\n", import_data->packages[i]);
                }
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
                print_ast_node(binary_expr_data->left, depth + 2, visited);

                print_indent(depth + 1);
                printf("Right Operand:\n");
                print_ast_node(binary_expr_data->right, depth + 2, visited);
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
                print_ast_node(node->children[i], depth + 1, visited);
            } else {
                printf("Child %zu is NULL\n", i);
            }
        }
    }
}

/**
 * @brief Prints the entire AST structure.
 *
 * This function initiates the AST printing process by calling `print_ast_node` 
 * for the root node and managing the visited nodes structure.
 *
 * @param root The root node of the AST.
 */
void print_ast(const AstNode *root) {
    printf("AST:\n");
    VisitedNodes visited;
    init_visited(&visited);
    print_ast_node(root, 0, &visited);
    free_visited(&visited);
}
