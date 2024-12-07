#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include <stdbool.h>
#include "definitions.h"
#include "frontend/lexer/core.h"
#include "frontend/types.h"

AstNode *create_ast_node(NodeType kind, void *data, int line, int column_start, int position_start, int column_end, int position_end);
void add_child_to_node(AstNode *parent, AstNode *child);
void *create_numeric_literal_data(double value);
void *create_identifier_data(const char *symbol);
void *create_binary_expr_data(AstNode *left, AstNode *right, char *op);
void *create_package_data(char *package);
void *create_property_data(char *key, AstNode *value);
void *create_variable_data(char *name, AstNode *value, bool isPtr, bool isConst, Type varType);
void *create_param_data(char *name, Type type, bool isConst, bool isPtr);
void free_ast_node(AstNode *node);

#endif // AST_H

