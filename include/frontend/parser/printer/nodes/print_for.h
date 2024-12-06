#ifndef PRINT_FOR_H
#define PRINT_FOR_H

#include "../../../ast/definitions.h"
#include "../visited.h"

void print_for(const AstNode *node, int depth, VisitedNodes *visited);

#endif // PRINT_FOR_H