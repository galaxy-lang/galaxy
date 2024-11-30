#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "utils.h"
#include "frontend/ast/definitions.h"
#include "frontend/parser/statements/parse_import_stmt.h"

AstNode *parse_import_stmt(Parser *parser) {
  int line = at(parser).line;
  int column_start = at(parser).column_start;
  int column_end = at(parser).column_end;
  int position_start = at(parser).position_start;
  int position_end = at(parser).position_end;

  eat(parser);
  expect(parser, TOKEN_OPAREN, "Expected \"(\".");

  ImportNode *import_data = MALLOC_S(sizeof(ImportNode));
  import_data->packages = NULL;
  import_data->package_count = 0;

  while (not_eof(parser) && at(parser).type != TOKEN_CPAREN) {
    int line_string = at(parser).line;
    int column_start_string = at(parser).column_start;
    int column_end_string = at(parser).column_end;
    int position_start_string = at(parser).position_start;
    int position_end_string = at(parser).position_end;

    char *package = expect(parser, TOKEN_STRING, "Expected a string").lexeme;
    
    column_end_string = at(parser).column_end - 1;
    position_end_string = at(parser).position_end - 1;

    import_data->packages = realloc(
        import_data->packages,
        sizeof(char *) * (import_data->package_count + 1)
    );

    if (!import_data->packages) {
        fprintf(stderr, "Error: Memory allocation failed for packages array\n");
        exit(EXIT_FAILURE);
    }

    import_data->packages[import_data->package_count] = strdup(package);
    import_data->package_count++;

    if (at(parser).type == TOKEN_COMMA){
      eat(parser);
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
      at(parser).column_end - 1,
      at(parser).position_end - 1
  );

  return import_node;
  
  free(import_data);
}