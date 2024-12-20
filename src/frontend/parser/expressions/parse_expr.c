#include "frontend/ast/definitions.h"
#include "frontend/parser/expressions/parse_expr.h"
#include "frontend/parser/expressions/parse_unary_expr.h"
#include "frontend/parser/expressions/parse_assignment_expr.h"
#include "frontend/parser/expressions/parse_primary_expr.h"
#include "frontend/parser/expressions/parse_call_member_expr.h"
#include "frontend/parser/expressions/binary_operations/parse_bitwise_expr.h"

AstNode *parse_expr(Parser *parser) {
  if (
    current_token(parser).type == TOKEN_MINUS
    || current_token(parser).type == TOKEN_NOT
    || current_token(parser).type == TOKEN_BITWISE_NOT
    || current_token(parser).type == TOKEN_INCREMENT
    || current_token(parser).type == TOKEN_DECREMENT
    || next_token(parser).type == TOKEN_INCREMENT
    || next_token(parser).type == TOKEN_DECREMENT
  ) return parse_unary_expr(parser);

  printf("Token: %s\n", current_token(parser).lexeme);
  if (next_token(parser).type == TOKEN_OPAREN) {
    AstNode *expr = parse_primary_expr(parser);
    expr = parse_call_member_expr(parser, expr);
    
    expect(parser, TOKEN_SEMICOLON, "Expected \";\".");

    return expr;
  }

  switch (next_token(parser).type){
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
