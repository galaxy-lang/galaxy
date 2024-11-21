#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../include/ast.h"

AstNode *ast_node_new(AstNodeType type, void *data) {
    AstNode *node = malloc(sizeof(AstNode));
    node->type = type;
    node->data = data;
    node->child_count = 0;
    node->capacity = 4;
    node->children = malloc(node->capacity * sizeof(AstNode *));
    return node;
}

void ast_push(AstNode *node, AstNode *child) {
    if (node->child_count >= node->capacity) {
        node->capacity *= 2;
        node->children = realloc(node->children, node->capacity * sizeof(AstNode *));
    }
    node->children[node->child_count++] = child;
}

void add_node_item(Program *program, AstNode *node) {
    ast_push(program->body, node);
    ast_node_free(node);
}

void ast_node_free(AstNode *node) {
    for (size_t i = 0; i < node->child_count; i++) {
        ast_node_free(node->children[i]);
    }
    free(node->children);
    free(node->data);
    free(node);
}

void program_init(Program *program) {
    program->body = malloc(sizeof(AstNode *));
    program->body->child_count = 0;
    program->body->capacity = 4;
    program->body->children = malloc(program->body->capacity * sizeof(AstNode *));
}

void program_free(Program *program) {
    for (size_t i = 0; i < program->body->child_count; i++) {
        ast_node_free(program->body->children[i]);
    }
    free(program->body->children);
    free(program->body);
}

void ast_free(AstNode *node) {
    ast_node_free(node);
}
