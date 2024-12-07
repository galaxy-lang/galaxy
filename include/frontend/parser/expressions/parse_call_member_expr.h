#ifndef PARSE_CALL_MEMBER_EXPR_H
#define PARSE_CALL_MEMBER_EXPR_H

#include "frontend/parser/core.h"

AstNode *parse_call_member_expr(Parser *parser, AstNode *statement);

#endif // PARSE_CALL_MEMBER_EXPR_H