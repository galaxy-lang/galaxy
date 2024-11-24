#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../include/ast/core.h"
#include "../../../include/ast/definitions.h"

/**
 * @brief Creates the AST (Abstract Syntax Tree) nodes.
 * 
 * This functions creates the nodes for the AST. 
 * 
 * @param kind: Defines the type of the node created used
 * to identify which constructor the node represents.
 * @param data: generic pointer used to store additional informations 
 * to the node.
 * @return node: Returns the pointer created and initialized.
 */

AstNode *create_ast_node(NodeType kind, void *data) {
  AstNode *node = malloc(sizeof(AstNode));
    node->kind = kind;
    node->data = data;
    node->children = NULL;
    node->child_count = 0;
    return node;
}

/**
 * @brief Creates the parent node to manage the child nodes.
 * 
 * This function manipulate the hierarchical data structure 
 * represented by the AST to allow the children node to be dinamically
 * added to the parent node.
 * 
 * @param parent: parent node.
 * @param child: child node.
 * @return void.
 */

void add_child_to_node(AstNode *parent, AstNode *child) {
       if (!parent || !child) {
        fprintf(stderr, "Error: Trying to add NULL node as child\n");
        return;
    }

    parent->children = realloc(
        parent->children,
        (parent->child_count + 1) * sizeof(AstNode *)
    );
    parent->children[parent->child_count++] = child;
}

/**
 * @brief Creates the numeric literal to be stored in data pointer.
 * 
 * This function creates a numeric literal to be stored in the data pointer
 * return by the value variable.
 * 
 * @param value: Store the fixed numeric literal stored by the pointer.
 * @return data: Returns the data with numeric literal number allocated.
 */

void *create_numeric_literal_data(double value) {
    NumericLiteralNode *data = malloc(sizeof(NumericLiteralNode));
    data->value = value;
    return data;
}

void *create_identifier_data(const char *symbol) {
    IdentifierNode *data = malloc(sizeof(IdentifierNode));
    data->symbol = strdup(symbol);
    return data;
}

void *create_binary_expr_data(AstNode *left, AstNode *right, const char *operator) {
    BinaryExprNode *data = malloc(sizeof(BinaryExprNode));
    data->left = left;
    data->right = right;
    data->operator = strdup(operator);
    return data;
}

/**
 * @brief Recursively frees memory allocated for an AST node and its children.
 *
 * This function deallocates memory for a given AST node, its data, and all its
 * child nodes. It handles different node types (program, numeric literal,
 * identifier, and binary expression) appropriately.
 *
 * @param node Pointer to the AstNode to be freed. If NULL, the function returns
 *             without doing anything.
 *
 * @return void This function does not return a value.
 */
void free_ast_node(AstNode *node) {
    if (!node) return;

    // Free children first
    for (size_t i = 0; i < node->child_count; i++) {
        free_ast_node(node->children[i]);
    }
    free(node->children);

    // Then handle the node's specific data
    switch (node->kind) {
        case NODE_PROGRAM: {
            ProgramNode *data = (ProgramNode *)node->data;
            if (data) {
                for (size_t i = 0; i < data->statement_count; i++) {
                    free_ast_node(data->statements[i]);
                }
                free(data->statements);
            }
            break;
        }
        case NODE_IDENTIFIER: {
            IdentifierNode *data = (IdentifierNode *)node->data;
            if (data && data->symbol) {
                free(data->symbol);
            }
            break;
        }
        case NODE_BINARY_EXPR: {
            BinaryExprNode *data = (BinaryExprNode *)node->data;
            if (data) {
                if (data->operator) {
                    free(data->operator);
                }
                // Note: left and right nodes are freed as children
            }
            break;
        }
        // Other cases (like NODE_NUMERIC_LITERAL) don't need special handling
    }

    // Finally, free the node's data and the node itself
    if (node->data) {
        free(node->data);
    }
    free(node);
}

