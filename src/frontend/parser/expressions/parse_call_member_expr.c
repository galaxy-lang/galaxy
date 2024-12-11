#include <string.h>
#include "frontend/ast/definitions.h"
#include "frontend/parser/expressions/parse_call_member_expr.h"
#include "frontend/parser/expressions/parse_call_expr.h"
#include "frontend/parser/expressions/parse_array_access_expr.h"
#include "frontend/parser/expressions/parse_primary_expr.h"
#include "frontend/parser/core.h"
#include "utils.h"

AstNode *parse_call_member_expr(Parser *parser, AstNode *statement) {
  AstNode *expr = NULL;

  if (statement) {
    expr = statement;
  } else {
    expr = parse_primary_expr(parser);
  }
  
  while (true) {
    switch (at(parser).type) {
      case TOKEN_DOT: {
        int line = at(parser).line;
        int column_start_member = at(parser).column_start;
        int column_end_member = at(parser).column_end;
        int position_start_member = at(parser).position_start;
        int position_end_member = at(parser).position_end;

        eat(parser);

        int column_start_val = at(parser).column_start;
        int column_end_val = at(parser).column_end;

        if (at(parser).type != TOKEN_IDENTIFIER && at(parser).type != TOKEN_NUMBER) {
          error(parser, "Expected identifier or integer on member expression.");
          exit(EXIT_FAILURE);
        }
        
        char *property = strdup(at(parser).lexeme);
        TokenType prop_type = eat(parser).type;

        int position_start_val = at(parser).position_start;
        int position_end_val = at(parser).position_end;
        
        MemberPropertyNode *prop_data = MALLOC_S(sizeof(MemberPropertyNode));
        prop_data->reference = property;
        prop_data->type = prop_type;

        AstNode *prop_node = create_ast_node(
          NODE_MEMBER_PROPERTY,
          prop_data,
          line,
          column_start_val,
          position_start_val,
          column_end_val,
          position_end_val
        );

        MemberNode *member_data = MALLOC_S(sizeof(MemberNode));
        member_data->member = expr;
        member_data->property = prop_node;

        AstNode *member_node = create_ast_node(
          NODE_MEMBER,
          member_data,
          line,
          column_start_member,
          position_start_member,
          column_end_member,
          position_end_member                
        );

        return member_node;
      }

      case TOKEN_OPAREN: {
        expr = parse_call_expr(parser, expr);
      } break;
              
      case TOKEN_OBRACKET: {
        expr = parse_array_access_expr(parser, expr);
      } break;

      case TOKEN_SEMICOLON: { 
        return expr;
      } 
      default: break;
    }
  }

  return expr;
}