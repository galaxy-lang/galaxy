#include <stdlib.h>
#include <stdbool.h>
#include "frontend/types.h"
#include "frontend/parser/statements/parse_function_declaration_stmt.h"
#include "frontend/parser/statements/parse_stmt.h"
#include "frontend/parser/types/parse_type.h"
#include "frontend/parser/core.h"
#include "utils.h"

AstNode *parse_for_stmt(Parser *parser) {
    int line = at(parser).line;
	int column_start = at(parser).column_start;
	int position_start = at(parser).position_start;

    eat(parser);
    expect(parser, TOKEN_OPAREN, "Expected \"(\".");

    Type var_type = TYPE_IMPLICIT;
    bool var_isConst = false;
    bool var_isPtr = false;
    char* variable = NULL;

    AstNode *iterator = NULL,
            *start = NULL,
            *stop = NULL,
            *updater = NULL;

    if (at(parser).type == TOKEN_CONST) {
        var_isConst = true;
        eat(parser);
    } 
    
    if (at(parser).type != TOKEN_IDENTIFIER) {
        var_type = parse_type(parser);

        if (at(parser).type == TOKEN_MUL) {
            var_isPtr = true;
            eat(parser);
        } 
    }

    variable = expect(parser, TOKEN_IDENTIFIER, "Expected identifier.").lexeme;

    if (at(parser).type == TOKEN_ASSIGN) {
        eat(parser);
        start = parse_expr(parser);

        expect(parser, TOKEN_SEMICOLON, "Expected \";\".");
        stop = parse_logical_expr(parser);

        expect(parser, TOKEN_SEMICOLON, "Expected \";\".");
        updater = parse_expr(parser);

    } else if (at(parser).type == TOKEN_COLON) {
        eat(parser);
        if (next(parser).type == TOKEN_RANGE || next(parser).type == TOKEN_IRANGE) {
            start = parse_expr(parser);
            eat(parser);
            stop = parse_expr(parser);
        } else {
            iterator = parse_expr(parser);
        }

    } else {
        error(parser, "Unexpected token in for statetement.");
		exit(EXIT_FAILURE);
    }
    
    expect(parser, TOKEN_CPAREN, "Expected \")\".");
    expect(parser, TOKEN_COLON, "Expected \":\".");

    ForNode *for_data = MALLOC_S(sizeof(ForNode));
    for_data->body = NULL;
    for_data->body_count = 0;
    for_data->variable = variable;
    for_data->var_type = var_type;
    for_data->var_isConst = var_isConst;
    for_data->var_isPtr = var_isPtr;
    for_data->start = start;
    for_data->stop = stop;
    for_data->updater = updater;
    for_data->iterator = iterator;

    while (not_eof(parser) && at(parser).type != TOKEN_END) {
        for_data->body = REALLOC_S(
            for_data->body, sizeof(AstNode *) * (for_data->body_count + 1)
        );

        for_data->body[for_data->body_count++] = parse_stmt(parser);
    }

    expect(parser, TOKEN_END, "Expected \"end\" to close for statement.");

    int column_end = at(parser).column_end - 1;
    int position_end = at(parser).position_end - 1;

    expect(parser, TOKEN_SEMICOLON, "Expected \";\".");
    
    AstNode *for_node = create_ast_node(
        NODE_FOR,
        for_data,
        line,
        column_start,
        position_start,
        column_end,
        position_end
    );

    return for_node;
}