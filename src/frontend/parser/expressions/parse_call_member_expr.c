#include <string.h>
#include "frontend/ast/definitions.h"
#include "frontend/parser/expressions/parse_call_member_expr.h"
#include "frontend/parser/expressions/parse_call_expr.h"
#include "frontend/parser/expressions/parse_array_access_expr.h"
#include "frontend/parser/expressions/parse_primary_expr.h"
#include "frontend/parser/expressions/parse_member_expr.h"
#include "frontend/parser/expressions/parse_expr.h"
#include "frontend/parser/core.h"
#include "utils.h"

AstNode *parse_call_member_expr(Parser *parser, AstNode *statement) {
  AstNode *expr = NULL;

  if (statement) {
    expr = statement;
  } else {
    expr = parse_member_expr(parser);
  }
  
  switch (current_token(parser).type) {
    case TOKEN_OPAREN: {
      expr = parse_call_expr(parser, expr);

      if (current_token(parser).type == TOKEN_SEMICOLON) {
        consume_token(parser);
        return expr;
      }
    } break;
            
    case TOKEN_OBRACKET: {
      expr = parse_array_access_expr(parser, expr);
    } break;

    case TOKEN_SEMICOLON: { 
      return expr;
    } 
  }

  return expr;
}