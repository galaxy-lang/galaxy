#ifndef PRINT_PROGRAM_H
#define PRINT_PROGRAM_H

#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/visited.h"

void print_program(const AstNode *node, int depth, VisitedNodes *visited);

#endif // PRINT_PROGRAM_H