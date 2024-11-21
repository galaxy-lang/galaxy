#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../../../../include/ast/definitions.h"
#include "../../../../include/parser/expressions/parse_expr.h"
#include "../../../../include/parser/expressions/binary_operations/parse_additive_expr.h"

AstNode *parse_expr(Parser *parser) {
  return parse_additive_expr(parser);
}
