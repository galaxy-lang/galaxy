#ifndef PRINT_VARIABLE_H
#define PRINT_VARIABLE_H

#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/visited.h"

void print_variable(const AstNode *node, int depth, VisitedNodes *visited);

#endif // PRINT_VARIABLE_H
