#include "frontend/parser/statements/parse_enum_stmt.h"

AstNode *parse_enum_stmt(Parser *parser)
{
  int line = current_token(parser).line;
  int column_start = current_token(parser).column_start;
  int position_start = current_token(parser).position_start;

  consume_token(parser);
  char *name = expect(parser, TOKEN_IDENTIFIER, "Expected identifier.").lexeme;
  expect(parser, TOKEN_COLON, "Expected \":\".");

  EnumNode *enum_data = MALLOC_S(sizeof(EnumNode));
  enum_data->items = MALLOC_S(sizeof(AstNode *));
  enum_data->name = name;
  enum_data->item_count = 0;

  while (not_eof(parser) && current_token(parser).type != TOKEN_END)
  {
    int line_item = current_token(parser).line;
    int column_start_item = current_token(parser).column_start;
    int position_start_item = current_token(parser).position_start;

    AstNode *assignment_node;

    switch (current_token(parser).type)
    {
    case TOKEN_IDENTIFIER:
      assignment_node = current_token(parser).lexeme;
      if (next_token(parser).type == TOKEN_ASSIGN)
      {
        consume_token(parser); // := aqui
        consume_token(parser); // valor aqui

        AssignmentNode *assignment_data = MALLOC_S(sizeof(AssignmentNode));
        assignment_data->left = assignment_node;
        assignment_data->value = current_token(parser).lexeme;

        assignment_node = create_ast_node(
            NODE_ASSIGNMENT,
            assignment_data,
            line_item,
            column_start_item,
            position_start_item,
            current_token(parser).column_end - 1,
            current_token(parser).position_end - 1);

        consume_token(parser); // consome o valor

        enum_data->items = REALLOC_S(enum_data->items, sizeof(AstNode *) * (enum_data->item_count + 1));
        enum_data->items[enum_data->item_count++] = assignment_node;
        break;
      }
      else
      {
        consume_token(parser); // consome o valor, não tem :=
      }
      break;

    case TOKEN_COMMA:
      consume_token(parser); // consome a ,
      break;

    default:
      const char *token_lexeme = current_token(parser).lexeme;
      const char *base_message = "Unexpected token inside enum statement. Lexeme: ";

      size_t size = strlen(base_message) + strlen(token_lexeme) + 1; // +1 para o '\0'
      char *error_message = MALLOC_S(size);

      strcpy(error_message, base_message);
      strcat(error_message, token_lexeme);

      error(parser, error_message);
      exit(EXIT_FAILURE);
      break;
    }

    enum_data->items = REALLOC_S(enum_data->items, sizeof(AstNode *) * (enum_data->item_count + 1));
    enum_data->items[enum_data->item_count++] = assignment_node;
  }

  int column_end = current_token(parser).column_end - 1;
  int position_end = current_token(parser).position_end - 1;

  expect(parser, TOKEN_END, "Expected \"end\".");
  consume_token(parser); // consome o end

  consume_token(parser); // consome o ;

  return create_ast_node(
      NODE_ENUM,
      enum_data,
      line,
      column_start,
      position_start,
      column_end,
      position_end);
}