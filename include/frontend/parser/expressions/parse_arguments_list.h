#ifndef PARSE_ARGUMENTS_LIST_H
#define PARSE_ARGUMENTS_LIST_H

#include "frontend/parser/core.h"

AstNode **parse_arguments_list(Parser *parser, int *arg_count);

#endif // PARSE_ARGUMENTS_LIST_H