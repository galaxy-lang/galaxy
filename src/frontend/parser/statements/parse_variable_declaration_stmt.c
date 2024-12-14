#include <stdlib.h>
#include <stdbool.h>
#include "frontend/parser/statements/parse_variable_declaration_stmt.h"
#include "frontend/parser/expressions/parse_expr.h"

AstNode *parse_variable_declaration_stmt(
    Parser *parser,
    bool isPtr,
    bool isConst,
    char *varType
) {
    int line = current_token(parser).line;
    int column_start = current_token(parser).column_start;
    int column_end = current_token(parser).column_end;
    int position_start = current_token(parser).position_start;
    int position_end = current_token(parser).position_end;

    char *name = expect(parser, TOKEN_IDENTIFIER, "Expected identifier.").lexeme;

    // Type *name; - Uninitialized variable declaration
    if (current_token(parser).type == TOKEN_SEMICOLON) {
        consume_token(parser);

        AstNode *variable_data = create_variable_data(
            name,
            NULL,
            isPtr,
            isConst,
            varType
        );

        AstNode *variable_node = create_ast_node(
            NODE_VARIABLE,
            variable_data,
            line,
            column_start,
            position_start,
            current_token(parser).column_end - 1,
            current_token(parser).position_end - 1
        );

        return variable_node;
    }

    // Type *name := value; - Initialized variable declaration
    expect(parser, TOKEN_ASSIGN, "Expected \":=\".");

    AstNode *value = parse_expr(parser);

    expect(parser, TOKEN_SEMICOLON, "Expected \";\".");

    AstNode *variable_data = create_variable_data(
        name,
        value,
        isPtr,
        isConst,
        varType
    );

    AstNode *variable_node = create_ast_node(
        NODE_VARIABLE,
        variable_data,
        line,
        column_start,
        position_start,
        current_token(parser).column_end - 1,
        current_token(parser).position_end - 1
    );

    return variable_node;
}

