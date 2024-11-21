#ifndef AST_H
#define AST_H

#include "lexer.h"

typedef enum {
    NODE_UNKNOWN,
    NODE_PROGRAM,
    NODE_STATEMENT,
    NODE_EXPRESSION
} AstNodeType;

typedef struct AstNode {
    AstNodeType type;
    void *data;
    struct AstNode **children;
    size_t child_count;
    size_t capacity;
} AstNode;

typedef struct {
    AstNode **children;
    size_t child_count;
    size_t capacity;
} Program;

AstNode *ast_node_new(AstNodeType type, void *data);
void ast_push(AstNode *node, AstNode *child);
void add_node_item(Program *program, AstNode *node);
void ast_node_free(AstNode *node);
void program_init(Program *program);
void program_free(Program *program);
void ast_free(AstNode *node);

#endif // AST_H
