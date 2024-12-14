#include "frontend/ast/definitions.h"
#include "frontend/parser/expressions/parse_args.h"
#include "frontend/parser/expressions/parse_arguments_list.h"
#include "frontend/parser/core.h"

AstNode **parse_args(Parser *parser, int *arg_count) {
  AstNode **args = NULL;
  *arg_count = 0;
  
  if (current_token(parser).type != TOKEN_CPAREN) {
    args = parse_arguments_list(parser, arg_count); 
  }
  
  expect(parser, TOKEN_CPAREN, "Expected \")\".");

  return args;
}