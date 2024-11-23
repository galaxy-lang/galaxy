#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include "../ast/definitions.h"

void print_ast(const AstNode *root);
const char* returnASTNodeName(NodeType node_type);
void print_indent(int depth);

#endif // AST_PRINTER_H
