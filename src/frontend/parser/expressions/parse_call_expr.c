#include "frontend/ast/definitions.h"
#include "frontend/parser/expressions/parse_call_expr.h"
#include "frontend/parser/expressions/parse_args.h"
#include "frontend/parser/core.h"
#include "utils.h"

AstNode *parse_call_expr(Parser *parser, AstNode *caller){
  int line = current_token(parser).line;
  int column_start = current_token(parser).column_start;
  int position_start = current_token(parser).position_start;
  
  expect(parser, TOKEN_OPAREN, "Expected \"(\".");
  
  int arg_count = 0;
  AstNode **args = parse_args(parser, &arg_count);
  
  int column_end = current_token(parser).column_end;
  int position_end = current_token(parser).position_end;

  CallNode *call_data = MALLOC_S(sizeof(CallNode));
  call_data->caller = caller;
  call_data->args = args;
  call_data->arg_count = arg_count;

  AstNode *call_node = create_ast_node(
    NODE_CALL,
    call_data,
    line,
    column_start,
    position_start,
    column_end,
    position_end
  );

  return call_node;
} 