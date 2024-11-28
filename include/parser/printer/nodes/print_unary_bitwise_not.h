#ifndef PRINT_UNARY_BITWISE_NOT_H
#define PRINT_UNARY_BITWISE_NOT_H

#include "../../../ast/definitions.h"
#include "../visited.h"

void print_unary_bitwise_not(const AstNode *node, int depth, VisitedNodes *visited);

#endif // PRINT_UNARY_BITWISE_NOT_H