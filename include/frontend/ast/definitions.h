#ifndef AST_DEFINITIONS_H
#define AST_DEFINITIONS_H

#include <stdlib.h>
#include <stdbool.h>
#include "frontend/lexer/core.h"

typedef enum
{
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
  NODE_WHILE,
  NODE_IF,
  NODE_DECORATOR,
  NODE_MEMBER,
  NODE_MEMBER_PROPERTY,
  NODE_CALL,
  NODE_ARRAY,
  NODE_ARRAY_ACCESS,
  NODE_TERNARY,
  NODE_STRING,
  NODE_RETURN,
  NODE_BOOLEAN_LITERAL,
  NODE_EXTERN,
  NODE_ENUM,
  NODE_SIZEOF,
} NodeType;

typedef struct AstNode AstNode;

typedef struct
{
  char *expression;
} SizeofNode;

typedef struct
{
  bool isConst;
  char *arg;
  bool isPtr;
} ExternNodeArgs;

typedef struct
{
  char *type, *extern_type, *identifier;
  ExternNodeArgs **args;
  size_t arg_count;
} ExternNode;

typedef struct
{
  char *value;
  char* ty; //bool
} BooleanLiteralNode;

typedef struct
{
  AstNode *value;
} ReturnNode;

typedef struct
{
  AstNode *condition;
  AstNode *consequent;
  AstNode *alternate;
} TernaryNode;

typedef struct
{
  AstNode *array;
  AstNode *index;
} ArrayAccessNode;

typedef struct
{
  AstNode *caller;
  AstNode **args;
  size_t arg_count;
  char* ty;
} CallNode;

typedef struct
{
  AstNode *object;
  AstNode *property;
} MemberPropertyNode;

typedef struct
{
  AstNode *member;
  AstNode *property;
} MemberNode;

typedef struct
{
  AstNode *decorator;
} DecoratorNode;

typedef struct
{
  AstNode **body;
  size_t body_count;
  char *variable;
  char *var_type;
  bool var_isPtr;
  AstNode *start;
  AstNode *stop;
  AstNode *updater;
  AstNode *iterator;
  bool is_parallel;
  char *schedule_policy; // "static", "dynamic", "guided"
  AstNode *num_threads;
} ForNode;

typedef struct
{
  AstNode *condition;
  AstNode **consequent;
  size_t consequent_count;
  AstNode **alternate;
  size_t alternate_count;
} IfNode;

typedef struct 
{
  AstNode *condition;
  AstNode** body;
  size_t body_count;
} WhileNode;

typedef struct
{
  char *string;
  char* ty; //deve ser string
} StringNode;

typedef struct
{
  AstNode **parameters;
  int parameter_count;
} ParametersNode;

typedef struct
{
  char *name;
  char *type;
  bool isConst;
  bool isPtr;
} ParameterNode;

typedef struct
{
  AstNode **body;
  size_t body_count;
  char *name;
  char *type;
  ParametersNode *parameters;
  bool isPtr;
} FunctionNode;

typedef struct
{
  double value;
  bool decimal;
  char* ty; //valor tem que ser: int se decimal = false, float caso contrario
} NumericLiteralNode;

typedef struct
{
  char *symbol;
} IdentifierNode;

typedef struct
{
  AstNode *left;
  AstNode *right;
  char *op;
  char* ty;
} BinaryExprNode;

typedef struct
{
  char *name;
  AstNode *value;
  bool isPtr;
  bool isConst;
  char *varType;
} VariableNode;

typedef struct
{
  char *package;
} PackageNode;

typedef struct
{
  AstNode *left;
  AstNode *value;
} AssignmentNode;

typedef struct
{
  AstNode **packages;
  size_t package_count;
} ImportNode;

typedef struct
{
  AstNode **properties;
  size_t property_count;
} ObjectNode;

typedef struct
{
  char *key;
  AstNode *value;
} PropertyNode;

typedef struct
{
  AstNode **statements;
  size_t statement_count;
} ProgramNode;

typedef struct
{
  AstNode *op;
} UnaryMinusExpr;

typedef struct
{
  AstNode *op;
} LogicalNotExpr;

typedef struct
{
  AstNode *op;
} UnaryBitwiseNotExpr;

typedef struct
{
  AstNode *op;
} PreIncrementExpr;

typedef struct
{
  AstNode *op;
} PreDecrementExpr;

typedef struct
{
  AstNode **elements;
  size_t element_count;
} ArrayNode;

typedef struct
{
  char *name;
  size_t item_count;
  AstNode **data;
} EnumNode;

struct AstNode
{
  NodeType kind;
  void *data;
  int line;
  int column_start;
  int column_end;
  int position_start;
  int position_end;
  AstNode **children;
  bool tocheck;
  size_t child_count;
};

#endif // AST_DEFINITIONS_H
