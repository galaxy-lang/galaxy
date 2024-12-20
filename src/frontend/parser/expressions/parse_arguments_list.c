#include "frontend/ast/definitions.h"
#include "frontend/parser/expressions/parse_expr.h"
#include "frontend/parser/expressions/parse_arguments_list.h"
#include "frontend/parser/core.h"
#include "utils.h"

AstNode **parse_arguments_list(Parser *parser, int *arg_count) {
  int capacity = 1;
  int count = 0;
  AstNode **args = MALLOC_S(sizeof(AstNode *) * capacity);

  args[count++] = parse_expr(parser);

  while (current_token(parser).type == TOKEN_COMMA) {
    consume_token(parser);

    if (current_token(parser).type != TOKEN_CPAREN) {
      capacity++;
      args = REALLOC_S(args, sizeof(AstNode *) * capacity);

      args[count++] = parse_expr(parser);
    } 
  }

  *arg_count = count;

  return args;
}