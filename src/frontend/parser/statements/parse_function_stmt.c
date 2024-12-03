#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "utils.h"
#include "frontend/types.h"
#include "frontend/parser/core.h"
#include "frontend/parser/types/parse_type.h"
#include "frontend/parser/statements/parse_stmt.h"
#include "frontend/ast/definitions.h"
#include "frontend/parser/statements/parse_function_stmt.h"

AstNode *parse_function_stmt(Parser *parser) {
    int line = at(parser).line;
    int column_start = at(parser).column_start;
    int column_end = at(parser).column_end;
    int position_start = at(parser).position_start;
    int position_end = at(parser).position_end;

    eat(parser);

    char *name = expect(parser, TOKEN_IDENTIFIER, "Expected identifier.").lexeme;
    expect(parser, TOKEN_OPAREN, "Expected \"(\".");

    ParametersNode *parameters_data = MALLOC_S(sizeof(ParametersNode));

    parameters_data->parameters = NULL;
    parameters_data->parameter_count = 0;

    while (not_eof(parser) && at(parser).type != TOKEN_CPAREN) {
        bool isConst = false;
        bool isPtr = false;    

        int line_param = at(parser).line;
        int column_start = at(parser).column;
        int position_start_param = at(parser).position_start;

        if(at(parser).type == TOKEN_CONST) {
            isConst = true;
            eat(parser);
        }
        Type type = parse_type(parser);

        if(at(parser).type == TOKEN_MUL) {
            isPtr = true;
            eat(parser);
        }
        
        char *param_name = expect(parser, TOKEN_IDENTIFIER "Expected identifier").lexeme;
        
        AstNode *param_data = create_param_data(
          param_name,
          type,
          isConst,
          isPtr
    );
        AstNode *param_node = create_ast_node(
          param_data,
          line_param,
          column_start_param,
          position_start_param,
          at(parser).column_end - 1,
          at(parser).position_end - 1
    );
        parameters_data->parameters = realloc(parameters_data->parameters, sizeof(AstNode *) * (parameters_data->parameter_count + 1));  
        
        if (!parameters_data->parameters) {
          fprintf(stderr, "Failed to reallocate memory for parameters");
          exit(EXIT_FAILURE);
    }
        parameters_data->parameters[parameters_data->parameter_count] = param_node;
        parameter_count++;
        
        if(at(parser).type == TOKEN_COMMA) {
          eat(parser);
      } else if(at(parser).type != TOKEN_CPAREN) {
          expect(parser, "Expected \", \" or \")\" after parameter.");
      }

     }
    expect(parser, "Expected \")\".");
    expect(parser, "Expected \"->\".");

    Type type = parse_type(parser);

    bool isPtr = false;
    if(at(parser).type == TOKEN_MUL) {
      eat(parser);
      isPtr = true;
  }
    expect(parser, TOKEN_COLON, "Expected \":\" after type");
    
    FunctionNode *function_data = MALLOC_S(sizeof(FunctionNode));
    function_data->body = NULL;
    function_data->body_count = 0;
    function_data->name = name;
    function_data->type = type;
    function_data->isPtr = isPtr;
    function_data->parameters = parameters_data;
}
