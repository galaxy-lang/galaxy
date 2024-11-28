#ifndef PRINT_OBJECT_H
#define PRINT_OBJECT_H

#include "../../../ast/definitions.h"
#include "../visited.h"

void print_object(const AstNode *node, int depth, VisitedNodes visited);

#endif // PRINT_OBJECT_H