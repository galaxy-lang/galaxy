#ifndef PRINT_ARRAY_ACCESS_H
#define PRINT_ARRAY_ACCESS_H

#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/visited.h"

void print_array_access(const AstNode *node, int depth, VisitedNodes *visited);

#endif // PRINT_ARRAY_ACCESS_H