#ifndef PRINT_LOGICAL_NOT_H
#define PRINT_LOGICAL_NOT_H

#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/visited.h"

void print_logical_not(const AstNode *node, int depth, VisitedNodes *visited);

#endif // PRINT_LOGICAL_NOT_H