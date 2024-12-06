#ifndef PRINT_BINARY_EXPR_H
#define PRINT_BINARY_EXPR_H

#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/visited.h"

void print_binary_expr(const AstNode *node, int depth, VisitedNodes *visited);

#endif // PRINT_BINARY_EXPR_H