#ifndef PRINT_PROPERTY_H
#define PRINT_PROPERTY_H

#include "../../../ast/definitions.h"
#include "../visited.h"

void print_property(const AstNode *node, int depth, VisitedNodes *visited);

#endif // PRINT_PROPERTY_H