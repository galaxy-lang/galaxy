#include "frontend/parser/expressions/parse_member_expr.h"
#include "frontend/parser/expressions/parse_expr.h"
#include "frontend/parser/expressions/parse_primary_expr.h"
#include "utils.h"

AstNode *parse_member_expr(Parser *parser) {
    int line = at(parser).line;
    int column_start_member = at(parser).column_start;
    int position_start_member = at(parser).position_start;

    AstNode *object = parse_primary_expr(parser);

    AstNode *current_member = object;

    while (at(parser).type == TOKEN_DOT) {
        eat(parser);

        if (at(parser).type != TOKEN_IDENTIFIER && at(parser).type != TOKEN_NUMBER) {
            error(parser, "Expected identifier or number after '.' in member expression.");
            exit(EXIT_FAILURE);
        }

        int column_start_val = at(parser).column_start;
        int position_start_val = at(parser).position_start;

        AstNode *property = parse_expr(parser);
        if (!property) {
            error(parser, "Failed to parse property in member expression.");
            exit(EXIT_FAILURE);
        }

        int column_end_val = at(parser).column_end - 1;
        int position_end_val = at(parser).position_end - 1;

        MemberPropertyNode *prop_data = MALLOC_S(sizeof(MemberPropertyNode));
        prop_data->object = current_member;
        prop_data->property = property;

        AstNode *prop_node = create_ast_node(
            NODE_MEMBER_PROPERTY,
            prop_data,
            line,
            column_start_val,
            position_start_val,
            column_end_val,
            position_end_val
        );

        current_member = prop_node;
    }

    MemberNode *member_data = MALLOC_S(sizeof(MemberNode));
    member_data->member = object;
    member_data->property = current_member;

    AstNode *member_node = create_ast_node(
        NODE_MEMBER,
        member_data,
        line,
        column_start_member,
        position_start_member,
        at(parser).column_end - 1,
        at(parser).position_end - 1
    );

    return member_node;
}
