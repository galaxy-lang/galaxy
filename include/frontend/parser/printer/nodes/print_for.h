#ifndef PRINT_FOR_H
#define PRINT_FOR_H

#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/visited.h"

void print_for(const AstNode *node, int depth, VisitedNodes *visited);

#endif // PRINT_FOR_H