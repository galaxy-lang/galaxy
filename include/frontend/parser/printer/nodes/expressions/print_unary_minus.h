#ifndef PRINT_MINUS_H
#define PRINT_MINUS_H

#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/visited.h"

void print_unary_minus(const AstNode *node, int depth, VisitedNodes *visited);

#endif