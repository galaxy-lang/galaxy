#include <stdio.h>
#include "frontend/ast/definitions.h"
#include "frontend/parser/statements/parse_decorator_stmt.h"
#include "frontend/parser/core.h"
#include "frontend/types.h"
#include "utils.h"

AstNode * parse_decorator_stmt(Parser *parser) {
  int line = at(parser).line; 
  int column_start = at(parser).column_start;
  int column_end = at(parser).column_end;

  eat(parser);

  AstNode *decorator = parse_call_member_expr(parser);

  int position_start = at(parser).position_start - 1;
  int position_end = at(parser).position_end - 1;
  
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