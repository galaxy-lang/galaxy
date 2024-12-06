#ifndef PRINT_PRE_DECREMENT_H
#define PRINT_PRE_DECREMENT_H

#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/visited.h"

void print_pre_decrement(const AstNode *node, int depth, VisitedNodes *visited);

#endif // PRINT_PRE_DECREMENT_H