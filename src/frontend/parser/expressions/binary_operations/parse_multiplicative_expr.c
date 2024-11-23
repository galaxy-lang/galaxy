#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../../../include/ast/definitions.h"
#include "../../../../../include/ast/core.h"
#include "../../../../../include/parser/core.h"
#include "../../../../../include/parser/expressions/parse_primary_expr.h"
#include "../../../../../include/parser/expressions/binary_operations/parse_multiplicative_expr.h"

AstNode *parse_multiplicative_expr(Parser *parser) {
    AstNode *left = parse_primary_expr(parser);

    while (at(parser).type == TOKEN_MUL || at(parser).type == TOKEN_DIV || at(parser).type == TOKEN_MODULUS) {
        char *operator = strdup(at(parser).lexeme);
        eat(parser);
        AstNode *right = parse_primary_expr(parser);

        AstNode *bin_expr = create_ast_node(NODE_BINARY_EXPR, create_binary_expr_data(left, right, operator));

        add_child_to_node(bin_expr, left);
        add_child_to_node(bin_expr, right);

        left = bin_expr;
    }

    return left;
}
