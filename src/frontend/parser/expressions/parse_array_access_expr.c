#include "frontend/ast/definitions.h"
#include "frontend/parser/expressions/parse_array_access_expr.h"
#include "frontend/parser/expressions/parse_expr.h"
#include "frontend/parser/expressions/parse_call_member_expr.h"
#include "frontend/parser/core.h"
#include "utils.h" 

AstNode *parse_array_access_expr(Parser *parser, AstNode *array) {
  int line = at(parser).line;
  int column_start = at(parser).column_start;
  int column_end = at(parser).column_end;
  int position_start = at(parser).position_start;
  int position_end = at(parser).position_end;

  int column_start_array = at(parser).column_start;
  int column_end_array = at(parser).column_end;
  int position_start_array = at(parser).position_start;
  int position_end_array = at(parser).position_end;

  if(at(parser).type == TOKEN_OBRACKET) {
    eat(parser);

    if(at(parser).type == TOKEN_CBRACKET) {
      error(parser, "Expected array index");
      exit(EXIT_FAILURE);
    }

    AstNode *index_expr = parse_expr(parser);

    expect(parser, TOKEN_CBRACKET, "Expected token \"]\".");

    column_end_array = at(parser).column_end - 1;
    position_end_array = at(parser).position_end - 1;
    
    if (at(parser).type == TOKEN_ASSIGN) {
      eat(parser);

      AstNode *value_expr = parse_expr(parser);

      column_end = at(parser).column_end - 1;
      position_end = at(parser).position_end - 1;

      expect(parser, TOKEN_SEMICOLON, "Expected token \";\".");

      ArrayAccessNode *array_access_data = MALLOC_S(sizeof(ArrayAccessNode));
      array_access_data->array = array;
      array_access_data->index = index_expr; 

      AstNode *array_access_node = create_ast_node(
        NODE_ARRAY_ACCESS,
        array_access_data,
        line,
        column_start_array,
        position_start_array,
        column_end_array,
        position_end_array
      );

      AssignmentNode *assignment_data = MALLOC_S(sizeof(AssignmentNode));
      assignment_data->left = array_access_node;
      assignment_data->value = value_expr;

      AstNode *assignment_node = create_ast_node(
        NODE_ASSIGNMENT,
        assignment_data,
        line,
        column_start,
        position_start,
        column_end,
        position_end
      );

      return assignment_node;

    } else if (
      at(parser).type == TOKEN_OPAREN
      || at(parser).type == TOKEN_DOT 
      || at(parser).type == TOKEN_OBRACKET
    ) {
      ArrayAccessNode *array_access_data = MALLOC_S(sizeof(ArrayAccessNode));
      array_access_data->array = array;
      array_access_data->index = index_expr; 

      AstNode *array_access_node = create_ast_node(
        NODE_ARRAY_ACCESS,
        array_access_data,
        line,
        column_start_array,
        position_start_array,
        column_end_array,
        position_end_array
      );
      
      return parse_call_member_expr(parser, array_access_node);
    } else {
      ArrayAccessNode *array_access_data = MALLOC_S(sizeof(ArrayAccessNode));
      array_access_data->array = array;
      array_access_data->index = index_expr; 

      AstNode *array_access_node = create_ast_node(
        NODE_ARRAY_ACCESS,
        array_access_data,
        line,
        column_start_array,
        position_start_array,
        column_end_array,
        position_end_array
      );
      return array_access_node;
    }
  } else {
    return parse_call_member_expr(parser, NULL); 
  } 
} 