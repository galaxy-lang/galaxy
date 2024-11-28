#ifndef PRINT_ASSIGNMENT_H
#define PRINT_ASSIGNMENT_H

#include "../../../ast/definitions.h"
#include "../visited.h"


void print_assignment(const AstNode *node, int depth, VisitedNodes *visited);

#endif // PRINT_ASSIGNMENT_H