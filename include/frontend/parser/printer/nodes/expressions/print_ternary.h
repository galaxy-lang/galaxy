#ifndef PRINT_TERNARY_H
#define PRINT_TERNARY_H

#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/visited.h"

void print_ternary(const AstNode *node, int depth, VisitedNodes *visited);

#endif // PRINT_TERNARY_H