#ifndef PARSE_ARRAY_ACCESS_EXPR_H
#define PARSE_ARRAY_ACCESS_EXPR_H

#include "frontend/parser/core.h"

AstNode *parse_array_access_expr(Parser *parser, AstNode *array);

#endif // PARSE_ARRAY_ACCESS_EXPR_H