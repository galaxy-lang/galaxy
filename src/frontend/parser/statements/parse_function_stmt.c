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
        expect(parser, TOKEN_CPAREN, "Expected \")\".");
        expect(parser, TOKEN_END, "Expected \"end\".");

        AstNode *function_node = create_ast_node(
            NODE_FUNCTION,
            parameters_data,
            line,
            column_start,
            position_start,
            at(parser).column_end - 1,
             // at(parser).position_end - 1
        );

        return function_node;
        
        free(parameters_data);
    }
}