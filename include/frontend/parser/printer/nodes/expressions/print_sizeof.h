#ifndef PRINT_SIZEOF_H
#define PRINT_SIZEOF_H

#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/visited.h"

void print_sizeof(const AstNode *node, int depth);

#endif // PRINT_SIZEOF_H