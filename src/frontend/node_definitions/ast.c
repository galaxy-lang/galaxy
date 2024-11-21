#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../include/ast/core.h"
#include "../../../include/ast/definitions.h"

AstNode *create_ast_node(NodeType kind, void *data) {
  AstNode *node = malloc(sizeof(AstNode));
    node->kind = kind;
    node->data = data;
    node->children = NULL;
    node->child_count = 0;
    return node;
}

void add_child_to_node(AstNode *parent, AstNode *child) {
    parent->children = realloc(
        parent->children,
        (parent->child_count + 1) * sizeof(AstNode *)
    );
    parent->children[parent->child_count++] = child;
}

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

void free_ast_node(AstNode *node) {
    if (!node) return;

    switch (node->kind) {
        case NODE_PROGRAM: {
            ProgramNode *data = (ProgramNode *)node->data;
            for (size_t i = 0; i < data->statement_count; i++) {
                free_ast_node(data->statements[i]);
            }
            free(data->statements);
            break;
        }
        case NODE_NUMERIC_LITERAL: {
            free(node->data);
            break;
        }
        case NODE_IDENTIFIER: {
            IdentifierNode *data = (IdentifierNode *)node->data;
            free(data->symbol);
            break;
        }
        case NODE_BINARY_EXPR: {
            BinaryExprNode *data = (BinaryExprNode *)node->data;
            free_ast_node(data->left);
            free_ast_node(data->right);
            free(data->operator);
            break;
        }
    }

    for (size_t i = 0; i < node->child_count; i++) {
        free_ast_node(node->children[i]);
    }

    free(node->children);
    free(node->data);
    free(node);
}

void free_all_ast(AstNode *root) {
    free_ast_node(root);
}

