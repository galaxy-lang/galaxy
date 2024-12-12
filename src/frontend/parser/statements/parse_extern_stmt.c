#include "frontend/parser/statements/parse_extern_stmt.h"
#include "frontend/parser/types/parse_type.h"
#include "utils.h"

AstNode *parse_extern_stmt(Parser *parser) {
  int line = at(parser).line;
  int column_start = at(parser).column_start;
  int position_start = at(parser).position_start;

  eat(parser);

  char *type = parse_type(parser);
  char *identifier = expect(parser, TOKEN_IDENTIFIER, "Expected identifier").lexeme;

  ExternNode *extern_data = MALLOC_S(sizeof(ExternNode));
  extern_data->args = NULL;
  extern_data->arg_count = 0;
  extern_data->type = type;
  extern_data->extern_type = "variable";
  extern_data->identifier = identifier;

  if (at(parser).type == TOKEN_OPAREN) {
    eat(parser);

    extern_data->extern_type = "function";

    while (not_eof(parser) && at(parser).type!=TOKEN_CPAREN) {
      char *type = eat(parser).lexeme;

      extern_data->args = REALLOC_S(
          extern_data->args,
          sizeof(ExternNode *) * (extern_data->arg_count + 1)
        );

      extern_data->args[extern_data->arg_count++] = type;

      if (at(parser).type == TOKEN_COMMA) {
        eat(parser);
      }
    }

    expect(parser, TOKEN_CPAREN, "Expected \")\".");
  }

  int column_end = at(parser).column_end;
  int position_end = at(parser).position_end;

  expect(parser, TOKEN_SEMICOLON, "Expected \";\".");

  AstNode *extern_node = create_ast_node(
    NODE_EXTERN,
    extern_data,
    line,
    column_start,
    position_start,
    column_end,
    position_end
  );

  return extern_node;
}