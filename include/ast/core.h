#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include <stdbool.h>
#include "definitions.h"

AstNode *create_ast_node(NodeType kind, void *data);
void add_child_to_node(AstNode *parent, AstNode *child);
void *create_numeric_literal_data(double value);
void *create_identifier_data(const char *symbol);
void *create_binary_expr_data(AstNode *left, AstNode *right, const char *operator);
void free_ast_node(AstNode *node);

#endif // AST_H

