#ifndef PARSE_ENUM_STMT_H
#define PARSE_ENUM_STMT_H

#include <string.h>
#include "frontend/ast/core.h"
#include "frontend/ast/definitions.h"
#include "frontend/parser/core.h"
#include "frontend/parser/expressions/parse_primary_expr.h"
#include "frontend/parser/expressions/parse_expr.h"
#include "frontend/parser/printer/print_ast.h"
#include "utils.h"

void enum_error(Parser *parser);
void insert_enum_item(EnumNode *enum_data, AstNode *node);
NumericLiteralNode *create_numeric_literal  (Parser *parser);
AstNode *parse_enum_stmt(Parser *parser);

#endif