#ifndef PRINT_UNARY_BITWISE_NOT_H
#define PRINT_UNARY_BITWISE_NOT_H

#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/visited.h"

void print_unary_bitwise_not(const AstNode *node, int depth, VisitedNodes *visited);

#endif // PRINT_UNARY_BITWISE_NOT_H