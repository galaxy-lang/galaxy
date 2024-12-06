#ifndef PRINT_PROPERTY_H
#define PRINT_PROPERTY_H

#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/visited.h"

void print_property(const AstNode *node, int depth, VisitedNodes *visited);

#endif // PRINT_PROPERTY_H