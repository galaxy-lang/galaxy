#ifndef PRINT_OBJECT_H
#define PRINT_OBJECT_H

#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/visited.h"

void print_object(const AstNode *node, int depth, VisitedNodes *visited);

#endif // PRINT_OBJECT_H