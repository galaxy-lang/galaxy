#ifndef PRINT_CALL_H
#define PRINT_CALL_H

#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/visited.h"

void print_call(const AstNode *node, int depth, VisitedNodes *visited);

#endif // PRINT_CALL_H