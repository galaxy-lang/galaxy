#ifndef PRINT_ARRAYS_H
#define PRINT_ARRAYS_H

#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/visited.h"

void print_arrays_expr(const AstNode *node, int depth, VisitedNodes *visited);

#endif // PRINT_ARRAYS_H