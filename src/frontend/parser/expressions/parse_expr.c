#include "frontend/ast/definitions.h"
#include "frontend/parser/expressions/parse_expr.h"
#include "frontend/parser/expressions/parse_unary_expr.h"
#include "frontend/parser/expressions/parse_assignment_expr.h"
#include "frontend/parser/expressions/binary_operations/parse_bitwise_expr.h"

AstNode *parse_expr(Parser *parser) {
  if (
    at(parser).type == TOKEN_MINUS
    || at(parser).type == TOKEN_NOT
    || at(parser).type == TOKEN_BITWISE_NOT
    || at(parser).type == TOKEN_INCREMENT
    || at(parser).type == TOKEN_DECREMENT
    || next(parser).type == TOKEN_INCREMENT
    || next(parser).type == TOKEN_DECREMENT
  ) return parse_unary_expr(parser);

  switch (next(parser).type){
    case TOKEN_BITWISE_AND:
    case TOKEN_BITWISE_OR:
    case TOKEN_BITWISE_XOR:
    case TOKEN_SHIFT_LEFT:
    case TOKEN_SHIFT_RIGHT:
    case TOKEN_PLUS:
    case TOKEN_MINUS:
    case TOKEN_MUL:
    case TOKEN_DIV:
    case TOKEN_MODULUS: return parse_bitwise_expr(parser);
    default: return parse_assignment_expr(parser);
  }
}
