#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "utils.h"
#include "frontend/ast/definitions.h"
#include "frontend/parser/statements/parse_import_stmt.h"

AstNode *parse_import_stmt(Parser *parser) {
  int line = current_token(parser).line;
  int column_start = current_token(parser).column_start;
  int column_end = current_token(parser).column_end;
  int position_start = current_token(parser).position_start;
  int position_end = current_token(parser).position_end;

  consume_token(parser);
  expect(parser, TOKEN_OPAREN, "Expected \"(\".");

  ImportNode *import_data = MALLOC_S(sizeof(ImportNode));
  import_data->packages = NULL;
  import_data->package_count = 0;

  while (not_eof(parser) && current_token(parser).type != TOKEN_CPAREN) {
    int line_string = current_token(parser).line;
    int column_start_string = current_token(parser).column_start;
    int column_end_string = current_token(parser).column_end;
    int position_start_string = current_token(parser).position_start;
    int position_end_string = current_token(parser).position_end;

    char *package = expect(parser, TOKEN_STRING, "Expected a string").lexeme;
    
    column_end_string = current_token(parser).column_end - 1;
    position_end_string = current_token(parser).position_end - 1;

    import_data->packages = realloc(
        import_data->packages,
        sizeof(char *) * (import_data->package_count + 1)
    );

    if (!import_data->packages) {
        fprintf(stderr, "Error: Memory allocation failed for packages array\n");
        exit(EXIT_FAILURE);
    }

    import_data->packages[import_data->package_count] = (AstNode*)strdup(package);
    import_data->package_count++;

    if (current_token(parser).type == TOKEN_COMMA){
      consume_token(parser);
    }
  }

  expect(parser, TOKEN_CPAREN, "Expected \")\".");
  expect(parser, TOKEN_SEMICOLON, "Expected \";\".");

  AstNode *import_node = create_ast_node(
      NODE_IMPORT,
      import_data,
      line,
      column_start,
      position_start,
      current_token(parser).column_end - 1,
      current_token(parser).position_end - 1
  );

  return import_node;
  
  free(import_data);
}