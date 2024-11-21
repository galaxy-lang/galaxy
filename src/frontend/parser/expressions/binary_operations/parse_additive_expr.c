#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../../../../../include/ast/definitions.h"
#include "../../../../../include/ast/core.h"
#include "../../../../../include/parser/core.h"
#include "../../../../../include/parser/expressions/binary_operations/parse_additive_expr.h"
#include "../../../../../include/parser/expressions/binary_operations/parse_multiplicative_expr.h"

AstNode *parse_additive_expr(Parser *parser) {
    AstNode *left = parse_multiplicative_expr(parser);

    while (at(parser).type == TOKEN_PLUS || at(parser).type == TOKEN_MINUS) {
        char *operator = strdup(at(parser).lexeme);
        eat(parser);
        AstNode *right = parse_multiplicative_expr(parser);

        AstNode *bin_expr = create_ast_node(NODE_BINARY_EXPR, NULL);
        bin_expr->data = operator;
        add_child_to_node(bin_expr, left);
        add_child_to_node(bin_expr, right);

        left = bin_expr;
    }

    return left;
}

