#ifndef PRINT_ENUM_H
#define PRINT_ENUM_H

#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/print_indent.h"
#include "frontend/parser/printer/print_ast.h"
#include "frontend/parser/printer/visited.h"

void print_enum(const AstNode *node, int depth, VisitedNodes *visited);

#endif // PRINT_ENUM_H