#ifndef PARSE_ENUM_STMT_H
#define PARSE_ENUM_STMT_H

#include <string.h>
#include "frontend/ast/core.h"
#include "frontend/ast/definitions.h"
#include "frontend/parser/core.h"
#include "frontend/parser/expressions/parse_primary_expr.h"
#include "frontend/parser/expressions/parse_expr.h"
#include "utils.h"

AstNode *parse_enum_stmt(Parser *parser);

#endif