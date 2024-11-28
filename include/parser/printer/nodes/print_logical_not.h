#ifndef PRINT_LOGICAL_NOT_H
#define PRINT_LOGICAL_NOT_H

#include "../../../ast/definitions.h"
#include "../visited.h"

void print_logical_not(const AstNode *node, int depth, VisitedNodes *visited);

#endif // PRINT_LOGICAL_NOT_H