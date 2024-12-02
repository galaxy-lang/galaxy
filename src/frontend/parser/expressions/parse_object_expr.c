#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "utils.h"
#include "frontend/ast/definitions.h"
#include "frontend/parser/expressions/parse_object_expr.h"
#include "frontend/parser/expressions/parse_expr.h"
#include "frontend/parser/expressions/binary_operations/parse_bitwise_expr.h"

AstNode *parse_object_expr(Parser *parser) {
  if (at(parser).type != TOKEN_OBRACE) {
    return parse_bitwiser_expr(parser);
  }

  int line = at(parser).line;
  int column_start = at(parser).column_start;
  int column_end = at(parser).column_end;
  int position_start = at(parser).position_start;
  int position_end = at(parser).position_end;

  eat(parser);

  ObjectNode *object_data = MALLOC_S(sizeof(ObjectNode));
  
  if (!object_data) {
    fprintf(stderr, "Error: Memory allocation failed for ObjectNode\n");
    exit(EXIT_FAILURE);
  }

  object_data->properties = NULL;
  object_data->property_count = 0;

  while (not_eof(parser) && at(parser).type != TOKEN_CBRACE) {
    int line_prop = at(parser).line;
    int column_start_prop = at(parser).column_start;
    int column_end_prop = at(parser).column_end;
    int position_start_prop = at(parser).position_start;
    int position_end_prop = at(parser).position_end;

    Token key = expect(parser, TOKEN_IDENTIFIER, "Expected identifier.");
    AstNode *value_node = NULL;

    if (at(parser).type == TOKEN_COLON){
      eat(parser);
      value_node = parse_expr(parser);
    }

    column_end_prop = at(parser).column_end - 1;
    position_end_prop = at(parser).position_end - 1;

    AstNode *property_data = create_property_data(key.lexeme, value_node);
    AstNode *property_node = create_ast_node(
        NODE_PROPERTY,
        property_data,
        line_prop,
        column_start_prop,
        position_start_prop,
        column_end_prop,
        position_end_prop
    );

    object_data->properties = realloc(
      object_data->properties,
      sizeof(AstNode *) * (object_data->property_count + 1)
    );

    if (!object_data->properties){
      fprintf(stderr, "Error: memory allocation failed for properties array.\n");
      exit(EXIT_FAILURE);
    }

    object_data->properties[object_data->property_count++] = property_node;

    if (at(parser).type == TOKEN_COMMA) {
      eat(parser);
    } else if (at(parser).type != TOKEN_CBRACE) {
        expect(parser, TOKEN_COMMA, "Expected \",\" or \"}\" after property.");
    }
  } 

  column_end = at(parser).column_end - 1;
  position_end = at(parser).position_end - 1;
  
  expect(parser, TOKEN_CBRACE, "Expected \"}\".");


  AstNode *object_node = create_ast_node(
    NODE_OBJECT, object_data, line, column_start, position_start,
    column_end, position_end
  );

  return object_node;
}   
