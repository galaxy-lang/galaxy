#ifndef PARSE_CALL_EXPR_H
#define PARSE_CALL_EXPR_H

#include "frontend/parser/core.h"

AstNode *parse_call_expr(Parser *parser, AstNode *caller);

#endif // PARSE_CALL_EXPR_H