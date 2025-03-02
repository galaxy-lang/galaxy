#ifndef PRINT_ARRAY_H
#define PRINT_ARRAY_H

#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/visited.h"

void print_array(const AstNode *node, int depth, VisitedNodes *visited);

#endif // PRINT_ARRAY_H