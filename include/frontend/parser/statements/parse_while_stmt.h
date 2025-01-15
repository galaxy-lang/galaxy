#ifndef PARSE_WHILE_STMT_H
#define PARSE_WHILE_STMT_H 

#include "frontend/parser/core.h"
#include <stdbool.h>

AstNode *parse_while_stmt(Parser *parser);

#endif // PARSE_WHILE_STMT_H