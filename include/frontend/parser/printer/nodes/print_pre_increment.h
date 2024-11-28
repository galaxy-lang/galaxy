#ifndef PRINT_PRE_INCREMENT_H
#define PRINT_PRE_INCREMENT_H

#include "../../../ast/definitions.h"
#include "../visited.h"

void print_pre_increment(const AstNode *node, int depth, VisitedNodes *visited);

#endif // PRINT_PRE_INCREMENT_H