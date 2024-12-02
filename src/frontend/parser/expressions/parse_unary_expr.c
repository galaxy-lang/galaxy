#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "frontend/lexer/core.h"
#include "frontend/parser/core.h"
#include "frontend/parser/expressions/parse_unary_expr.h"
#include "frontend/parser/expressions/binary_operations/parse_bitwise_expr.h"

AstNode *parse_unary_expr(Parser *parser) {
  int line = at(parser).line;
  int column_start = at(parser).column_start;
  int column_end = at(parser).column_end;
  int position_start = at(parser).position_start;
  int position_end = at(parser).position_end;

  AstNode *expr;

  switch (at(parser).type) {
    case TOKEN_MINUS: {
      eat(parser);
      AstNode *op = parse_unary_expr(parser);

      column_end = at(parser).column_end - 1;
      position_end = at(parser).position_end - 1;

      UnaryMinusExpr *data = MALLOC_S(sizeof(UnaryMinusExpr));
      data->op = op;

      AstNode *unary_minus_expr_node = create_ast_node(
        NODE_UNARY_MINUS,
        data,
        line,
        column_start, 
        position_start,
        column_end,
        position_end
      );
      
      expr = unary_minus_expr_node;
      break;
    }
    case TOKEN_NOT: {
      eat(parser);
      AstNode *op = parse_unary_expr(parser);

      column_end = at(parser).column_end - 1;
      position_end = at(parser).position_end - 1;

      LogicalNotExpr *data = MALLOC_S(sizeof(LogicalNotExpr));
      data->op = op;

      AstNode *logical_not_expr_node = create_ast_node(
        NODE_LOGICAL_NOT,
        data,
        line,
        column_start, 
        position_start,
        column_end,
        position_end
      );

      expr = logical_not_expr_node;
      break;
    }
    
    case TOKEN_BITWISE_NOT: {
      eat(parser);
      AstNode *op = parse_unary_expr(parser);
      
      column_end = at(parser).column_end - 1; 
      position_end = at(parser).position_end - 1;

      UnaryBitwiseNotExpr *data = MALLOC_S(sizeof(UnaryBitwiseNotExpr));
      data->op = op;

      AstNode *bitwise_not_expr_node = create_ast_node(
        NODE_UNARY_BITWISE_NOT,
        data,
        line,
        column_start, 
        position_start,
        column_end,
        position_end
      );
      
      expr = bitwise_not_expr_node;
      break;
    }
    
    case TOKEN_INCREMENT: {
      eat(parser);
      AstNode *op = parse_unary_expr(parser);

      column_end = at(parser).column_end - 1;
      position_end = at(parser).position_end - 1;

      PreIncrementExpr *data = MALLOC_S(sizeof(PreIncrementExpr));
      data->op = op;

      AstNode *pre_increment_expr_node = create_ast_node(
        NODE_PRE_INCREMENT,
        data,
        line,
        column_start, 
        position_start,
        column_end,
        position_end
      );
      
      expr = pre_increment_expr_node;
      break;
    }
    
    case TOKEN_DECREMENT: {
      eat(parser);
      AstNode *op = parse_unary_expr(parser);

      column_end = at(parser).column_end - 1;
      position_end = at(parser).position_end - 1;

      PreDecrementExpr *data = MALLOC_S(sizeof(PreDecrementExpr));
      data->op = op;

      AstNode *pre_decrement_expr_node = create_ast_node(
        NODE_PRE_DECREMENT,
        data,
        line,
        column_start, 
        position_start,
        column_end,
        position_end
      );
      
      expr = pre_decrement_expr_node;
      break;
    }   
    default: {
      AstNode *bitwise = parse_bitwise_expr(parser);
      // TODO: verificação de postfix

      expr = bitwise;
      break;
    }
  }

  return expr;
}
