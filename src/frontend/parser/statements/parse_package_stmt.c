#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "frontend/ast/definitions.h"
#include "frontend/parser/statements/parse_package_stmt.h"

AstNode *parse_package_stmt(Parser *parser) {
  int line = at(parser).line;
  int column_start = at(parser).column_start;
  int column_end = at(parser).column_end;
  int position_start = at(parser).position_start;
  int position_end = at(parser).position_end;

  eat(parser);
  Token package = expect(parser, TOKEN_IDENTIFIER, "Identifier expected at package name");

  expect(parser, TOKEN_SEMICOLON, "Expected \";\".");

  column_end = at(parser).column_end - 1;
  position_end = at(parser).position_end - 1;

  AstNode *package_data = create_package_data(package.lexeme);

  AstNode *node = create_ast_node(
    NODE_PACKAGE,
    package_data,
    line,
    column_start,
    position_start,
    column_end,
    position_end
  );

  return node;
}
