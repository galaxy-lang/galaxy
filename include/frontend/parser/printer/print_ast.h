#ifndef PRINT_AST_NODE_H
#define PRINT_AST_NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/print_indent.h"
#include "frontend/parser/printer/visited.h"
#include "frontend/parser/printer/nodes/print_program.h"

#include "frontend/parser/printer/nodes/expressions/print_array.h"
#include "frontend/parser/printer/nodes/expressions/print_array_access.h"
#include "frontend/parser/printer/nodes/expressions/print_assignment.h"
#include "frontend/parser/printer/nodes/expressions/print_binary_expr.h"
#include "frontend/parser/printer/nodes/expressions/print_identifier.h"
#include "frontend/parser/printer/nodes/expressions/print_property.h"
#include "frontend/parser/printer/nodes/expressions/print_object.h"
#include "frontend/parser/printer/nodes/expressions/print_numeric_literal.h"
#include "frontend/parser/printer/nodes/expressions/print_logical_not.h"
#include "frontend/parser/printer/nodes/expressions/print_unary_minus.h"
#include "frontend/parser/printer/nodes/expressions/print_unary_bitwise_not.h"
#include "frontend/parser/printer/nodes/expressions/print_pre_decrement.h"
#include "frontend/parser/printer/nodes/expressions/print_pre_increment.h"
#include "frontend/parser/printer/nodes/expressions/print_string.h"
#include "frontend/parser/printer/nodes/expressions/print_boolean.h"
#include "frontend/parser/printer/nodes/expressions/print_call.h"
#include "frontend/parser/printer/nodes/expressions/print_return.h"
#include "frontend/parser/printer/nodes/expressions/print_ternary.h"
#include "frontend/parser/printer/nodes/expressions/print_sizeof.h"

#include "frontend/parser/printer/nodes/statements/print_import.h"
#include "frontend/parser/printer/nodes/statements/print_package.h"
#include "frontend/parser/printer/nodes/statements/print_variable.h"
#include "frontend/parser/printer/nodes/statements/print_function.h"
#include "frontend/parser/printer/nodes/statements/print_for.h"
#include "frontend/parser/printer/nodes/statements/print_while.h"
#include "frontend/parser/printer/nodes/statements/print_if.h"
#include "frontend/parser/printer/nodes/statements/print_extern.h"
#include "frontend/parser/printer/nodes/statements/print_enum.h"
#include "frontend/parser/printer/nodes/statements/print_while.h"

void print_ast_node(const AstNode *node, int depth, VisitedNodes *visited);
const char *returnASTNodeName(NodeType node_type);
void print_ast(const AstNode *root);

#endif // PRINT_AST_NODE_H