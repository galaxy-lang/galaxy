#ifndef AST_DEFINITIONS_H
#define AST_DEFINITIONS_H

#include <stdlib.h>
#include <stdbool.h>
#include "../lexer/core.h"
#include "frontend/types.h"

typedef enum {
    NODE_PROGRAM,
    NODE_NUMERIC_LITERAL,
    NODE_IDENTIFIER,
    NODE_BINARY_EXPR,
    NODE_IMPORT,
    NODE_PACKAGE,
    NODE_ASSIGNMENT,
    NODE_OBJECT,
    NODE_PROPERTY,
    NODE_UNARY_MINUS,
    NODE_LOGICAL_NOT,
    NODE_UNARY_BITWISE_NOT,
    NODE_PRE_INCREMENT,
    NODE_PRE_DECREMENT,
    NODE_VARIABLE,
    NODE_FUNCTION,
    NODE_PARAMETER,
    NODE_FOR,
} NodeType;

typedef struct AstNode AstNode;

typedef struct {
    AstNode **body;
    size_t body_count;
    char *variable;
    Type var_type;
    bool var_isConst, var_isPtr;
    AstNode *start;          // may be null, excluding iterator
    AstNode *stop;           // may be null, excluding iterator
    AstNode *updater;        // may be null, excluding iterator
    AstNode *iterator;       // may be null, excluding both start, stop and updater
} ForNode;

typedef struct {
    AstNode **parameters;
    int parameter_count;
} ParametersNode;

typedef struct {
    char *name;
    Type type;
    bool isConst;
    bool isPtr;
} ParameterNode;

typedef struct {
    AstNode **body;
    size_t body_count;
    char *name;
    Type type;
    ParametersNode *parameters;
    bool isConst;
    bool isPtr;
} FunctionNode;

typedef struct {
    double value;
    bool decimal;
} NumericLiteralNode;

typedef struct {
    char *symbol;
} IdentifierNode;

typedef struct {
    AstNode *left;
    AstNode *right;
    char *operator;
} BinaryExprNode;

typedef struct {
    char *name;
    AstNode *value;
    bool isPtr;
    bool isConst;
    Type varType;
} VariableNode;

typedef struct {
    char *package;
} PackageNode;

typedef struct {
    AstNode *left;
    AstNode *value;
} AssignmentNode;

typedef struct {
    AstNode **packages;
    size_t package_count;
} ImportNode;

typedef struct {
    AstNode **properties;
    size_t property_count;
} ObjectNode;

typedef struct {
    char *key;
    AstNode *value;
} PropertyNode;

typedef struct {
    AstNode **statements;
    size_t statement_count;
} ProgramNode;

typedef struct {
    AstNode *op;
} UnaryMinusExpr;

typedef struct {
    AstNode *op;
} LogicalNotExpr;

typedef struct {
    AstNode *op;
} UnaryBitwiseNotExpr;

typedef struct {
    AstNode *op;
} PreIncrementExpr;

typedef struct {
    AstNode *op;
} PreDecrementExpr;

struct AstNode {
    NodeType kind;
    void *data;
    int line;
    int column_start;
    int column_end;
    int position_start;
    int position_end;
    AstNode **children;
    size_t child_count;
};

#endif // AST_DEFINITIONS_H
