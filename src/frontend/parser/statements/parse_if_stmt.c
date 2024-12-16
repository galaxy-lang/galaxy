#include "frontend/parser/statements/parse_if_stmt.h"
#include "frontend/parser/expressions/binary_operations/parse_logical_expr.h"
#include "frontend/parser/statements/parse_stmt.h"
#include "utils.h"

AstNode *parse_if_stmt(Parser *parser) {
  int line = current_token(parser).line;
  int column_start = current_token(parser).column_start;
  int position_start = current_token(parser).position_start;

  consume_token(parser);
  expect(parser, TOKEN_OPAREN, "Expected \"(\".");

  AstNode *condition = parse_logical_expr(parser);

  expect(parser, TOKEN_CPAREN, "Expected \")\".");
  expect(parser, TOKEN_COLON, "Expected \":\".");

  IfNode *if_data = MALLOC_S(sizeof(IfNode));
  if_data->condition = condition;
  if_data->consequent = NULL;
  if_data->consequent_count = 0;
  if_data->alternate = NULL;
  if_data->alternate_count = 0;

  while (not_eof(parser) &&
    current_token(parser).type != TOKEN_ELIF &&
    current_token(parser).type != TOKEN_ELSE &&
    current_token(parser).type != TOKEN_END
  ) {
    if_data->consequent = REALLOC_S(
      if_data->consequent,
      sizeof(AstNode *) * (if_data->consequent_count + 1)
    );
    if_data->consequent[if_data->consequent_count++] = parse_stmt(parser);
  }

  while (not_eof(parser) && current_token(parser).type != TOKEN_END) {
    if (current_token(parser).type == TOKEN_ELIF) {
      consume_token(parser);
      expect(parser, TOKEN_OPAREN, "Expected \"(\".");
      
      AstNode *elif_condition = parse_logical_expr(parser);

      expect(parser, TOKEN_CPAREN, "Expected \")\".");
      expect(parser, TOKEN_COLON, "Expected \":\".");

      IfNode *elif_data = MALLOC_S(sizeof(IfNode));
      elif_data->condition = elif_condition;
      elif_data->consequent = NULL;
      elif_data->consequent_count = 0;
      elif_data->alternate = NULL;
      elif_data->alternate_count = 0;

      while (not_eof(parser) && current_token(parser).type != TOKEN_ELIF &&
             current_token(parser).type != TOKEN_ELSE && current_token(parser).type != TOKEN_END) {
        elif_data->consequent = REALLOC_S(
          elif_data->consequent,
          sizeof(AstNode *) * (elif_data->consequent_count + 1)
        );
        elif_data->consequent[elif_data->consequent_count++] = parse_stmt(parser);
      }

      if_data->alternate = REALLOC_S(
        if_data->alternate,
        sizeof(AstNode *) * (if_data->alternate_count + 1)
      );
      if_data->alternate[if_data->alternate_count++] = create_ast_node(
        NODE_IF,
        elif_data,
        line,
        column_start,
        position_start,
        current_token(parser).column_end - 1,
        current_token(parser).position_end - 1
      );
    } else if (current_token(parser).type == TOKEN_ELSE) {
      consume_token(parser);

      if (current_token(parser).type == TOKEN_IF) {
        consume_token(parser);

        expect(parser, TOKEN_COLON, "Expected \":\".");

        IfNode *else_if_data = MALLOC_S(sizeof(IfNode));
        else_if_data->condition = NULL;
        else_if_data->consequent = NULL;
        else_if_data->consequent_count = 0;
        else_if_data->alternate = NULL;
        else_if_data->alternate_count = 0;

        while (not_eof(parser) && current_token(parser).type != TOKEN_END &&
               current_token(parser).type != TOKEN_ELIF && current_token(parser).type != TOKEN_ELSE) {
          else_if_data->consequent = REALLOC_S(
            else_if_data->consequent,
            sizeof(AstNode *) * (else_if_data->consequent_count + 1)
          );
          else_if_data->consequent[else_if_data->consequent_count++] = parse_stmt(parser);
        }

        if_data->alternate = REALLOC_S(
          if_data->alternate,
          sizeof(AstNode *) * (if_data->alternate_count + 1)
        );
        if_data->alternate[if_data->alternate_count++] = create_ast_node(
          NODE_IF,
          else_if_data,
          line,
          column_start,
          position_start,
          current_token(parser).column_end - 1,
          current_token(parser).position_end - 1
        );
      } else {
        expect(parser, TOKEN_COLON, "Expected \":\".");

        while (not_eof(parser) && current_token(parser).type != TOKEN_END) {
          if_data->alternate = REALLOC_S(
            if_data->alternate,
            sizeof(AstNode *) * (if_data->alternate_count + 1)
          );
          if_data->alternate[if_data->alternate_count++] = parse_stmt(parser);
        }
        break;
      }
    }
  }

  expect(parser, TOKEN_END, "Expected \"end\".");
  expect(parser, TOKEN_SEMICOLON, "Expected \";\".");

  AstNode *if_node = create_ast_node(
    NODE_IF,
    if_data,
    line,
    column_start,
    position_start,
    current_token(parser).column_end - 1,
    current_token(parser).position_end - 1
  );

    return if_node;
}
