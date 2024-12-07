#ifndef PARSE_ARGS_H
#define PARSE_ARGS_H

#include "frontend/parser/core.h"

AstNode **parse_args(Parser *parser, int *arg_count);

#endif // PARSE_ARGS_H