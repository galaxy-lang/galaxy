#ifndef PARSE_VARIABLE_DECLARATION_STMT_H
#define PARSE_VARIABLE_DECLARATION_STMT_H

#include "frontend/parser/core.h"
#include <stdbool.h>

AstNode *parse_variable_declaration_stmt(Parser *parser, bool isPtr, bool isConst, char *varType);

#endif // PARSE_VARIABLE_DECLARATION_STMT_H
