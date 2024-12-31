#include "frontend/parser/statements/parse_enum_stmt.h"

void enum_error(Parser *parser)
{
  const char *token_lexeme = current_token(parser).lexeme;
  const char *base_message = "Unexpected token inside enum statement. Lexeme: ";
  size_t size = strlen(base_message) + strlen(token_lexeme) + 1; // +1 para o '\0'
  char *error_message = MALLOC_S(size);

  strcpy(error_message, base_message);
  strcat(error_message, token_lexeme);
  error(parser, error_message);
  free(error_message);
  exit(EXIT_FAILURE);
}

void insert_enum_item(EnumNode *enum_data, AstNode *node)
{
  enum_data->data = REALLOC_S(enum_data->data, sizeof(AstNode *) * (enum_data->item_count + 1));
  enum_data->data[enum_data->item_count++] = node;
}

NumericLiteralNode *create_numeric_literal(Parser *parser)
{
  NumericLiteralNode *numeric_data = MALLOC_S(sizeof(NumericLiteralNode));
  Token token = consume_token(parser);
  char *value = expect(parser, TOKEN_NUMBER, "Expected number.").lexeme; // number value

  if (strcmp(token.message, "decimal") == 0)
  {
    numeric_data->value = strtod(value, NULL);
    numeric_data->decimal = true;
  }
  else
  {
    numeric_data->value = atoi(value);
    numeric_data->decimal = false;
  }

  return numeric_data;
}

AstNode *parse_enum_stmt(Parser *parser)
{
  int line = current_token(parser).line;
  int column_start = current_token(parser).column_start;
  int position_start = current_token(parser).position_start;
  int column_end = current_token(parser).column_end - 1;
  int position_end = current_token(parser).position_end - 1;

  consume_token(parser);
  char *name = expect(parser, TOKEN_IDENTIFIER, "Expected identifier.").lexeme;
  expect(parser, TOKEN_COLON, "Expected \":\".");

  EnumNode *enum_data = MALLOC_S(sizeof(EnumNode));
  enum_data->data = MALLOC_S(sizeof(AstNode *));
  enum_data->name = name;
  enum_data->item_count = 0;

  while (not_eof(parser) && current_token(parser).type != TOKEN_END)
  {
    char *lexeme_debbug = current_token(parser).lexeme;

    int line_item = current_token(parser).line;
    int column_start_item = current_token(parser).column_start;
    int position_start_item = current_token(parser).position_start;

    AstNode *node = NULL;

    switch (current_token(parser).type)
    {
    case TOKEN_IDENTIFIER:
      IdentifierNode *identifier = create_identifier_data(current_token(parser).lexeme);

      AstNode *identifier_node = create_ast_node(
          NODE_IDENTIFIER,
          identifier,
          line,
          column_start,
          position_start,
          column_end,
          position_end);

      node = identifier_node; // add node value identifier

      if (next_token(parser).type == TOKEN_ASSIGN)
      {
        insert_enum_item(enum_data, node);
        consume_token(parser); // skip token_identifier

        // parse numeric
        NumericLiteralNode *numeric_data = create_numeric_literal(parser);
        AstNode *numeric_node = create_ast_node(
            NODE_NUMERIC_LITERAL,
            numeric_data,
            line,
            column_start,
            position_start,
            column_end,
            position_end);

        node = numeric_node; // add node value numeric
      }
      else
      {
        consume_token(parser); // skip token_identifier
      }
      break;

    case TOKEN_COMMA:
      consume_token(parser); // consome a ,
      break;

    default:
      enum_error(parser);
      break;
    }

    insert_enum_item(enum_data, node);
  }

  char *lexeme = NULL;

  expect(parser, TOKEN_END, "Expected \"end\".");
  lexeme = current_token(parser).lexeme;
  consume_token(parser); // consome o end

  lexeme = current_token(parser).lexeme;

  consume_token(parser); // consome o ;
  lexeme = current_token(parser).lexeme;

  AstNode *enum_node = create_ast_node(
      NODE_ENUM,
      enum_data,
      line,
      column_start,
      position_start,
      column_end,
      position_end);

  return enum_node;
}