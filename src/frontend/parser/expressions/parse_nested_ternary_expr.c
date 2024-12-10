#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "frontend/lexer/core.h"
#include "frontend/parser/core.h"
#include "frontend/parser/expressions/parse_ternary_expr.h"
#include "frontend/parser/expressions/parse_expr.h"
#include "frontend/parser/expressions/parse_nested_ternary_expr.h"

AstNode *parse_nested_ternary_expr(Parser *parser) {
  if (at(parser).type == TOKEN_OPAREN) {
    eat(parser);

    AstNode *expr = parse_ternary_expr(parser);

    expect(parser, TOKEN_CPAREN, "Expected \")\".");

    return expr;
  } else {
    return parse_expr(parser);
  }
}