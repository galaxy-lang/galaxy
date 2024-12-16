#ifndef PRINT_IF_H
#define PRINT_IF_H

#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/visited.h"

void print_if(const AstNode *node, int depth, VisitedNodes *visited);

#endif // PRINT_FUNCTION_H
