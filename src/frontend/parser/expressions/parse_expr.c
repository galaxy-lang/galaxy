#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../../../../include/ast/definitions.h"
#include "../../../../include/parser/expressions/parse_expr.h"
#include "../../../../include/parser/expressions/parse_unary_expr.h"
#include "../../../../include/parser/expressions/parse_assignment_expr.h"
#include "../../../../include/parser/expressions/binary_operations/parse_additive_expr.h"

AstNode *parse_expr(Parser *parser) {
  if (
    at(parser).type == TOKEN_MINUS
    || at(parser).type == TOKEN_NOT
    || at(parser).type == TOKEN_BITWISE_NOT
    || at(parser).type == TOKEN_INCREMENT
    || at(parser).type == TOKEN_DECREMENT
  ) {
    return parse_unary_expr(parser);
  } else {
    return parse_assignment_expr(parser);
  }
}
