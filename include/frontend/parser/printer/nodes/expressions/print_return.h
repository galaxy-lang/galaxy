#ifndef PRINT_RETURN_H
#define PRINT_RETURN_H

#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/visited.h"

void print_return(const AstNode *node, int depth, VisitedNodes *visited);

#endif // PRINT_RETURN_H