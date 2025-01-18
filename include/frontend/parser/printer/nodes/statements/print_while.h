#ifndef PRINT_WHILE_H
#define PRINT_WHILE_H

#include <stdio.h>
#include <stdlib.h>
#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/print_indent.h"
#include "frontend/parser/printer/print_ast.h"
#include "frontend/parser/printer/visited.h"

void print_while(const AstNode *node, int depth, VisitedNodes *visited);

#endif // PRINT_WHILE_H