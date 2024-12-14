#include <stdio.h>
#include "frontend/ast/definitions.h"
#include "frontend/parser/statements/parse_decorator_stmt.h"
#include "frontend/parser/expressions/parse_call_member_expr.h"
#include "frontend/parser/core.h"
#include "utils.h"

AstNode * parse_decorator_stmt(Parser *parser) {
  int line = current_token(parser).line; 
  int column_start = current_token(parser).column_start;
  int column_end = current_token(parser).column_end;

  consume_token(parser);

  AstNode *decorator = parse_call_member_expr(parser, NULL);

  int position_start = current_token(parser).position_start - 1;
  int position_end = current_token(parser).position_end - 1;
  
  expect(parser, TOKEN_SEMICOLON, "Expected \";\".");

  AstNode *decorator_node = create_ast_node(
    NODE_DECORATOR,
    decorator,
    line,
    column_start,
    position_start,
    column_end,
    position_end
  );
  
  return decorator_node;
}