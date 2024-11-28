#ifndef PRINT_BINARY_EXPR_H
#define PRINT_BINARY_EXPR_H

#include "../../../ast/definitions.h"
#include "../visited.h"

void print_binary_expr(const AstNode *node, int depth, VisitedNodes *visited);

#endif // PRINT_BINARY_EXPR_H