#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "frontend/ast/definitions.h"
#include "frontend/ast/core.h"
#include "frontend/parser/core.h"
#include "frontend/parser/expressions/binary_operations/parse_additive_expr.h"
#include "frontend/parser/expressions/binary_operations/parse_multiplicative_expr.h"

/**
 * @brief Parses an additive expression.
 *
 * This function is responsible for parsing additive expressions, which consist of terms that are combined using 
 * the addition (`+`) or subtraction (`-`) operators. The function starts by parsing the left operand using 
 * `parse_multiplicative_expr`, then repeatedly parses the right operand and operator (either `+` or `-`) 
 * until no more additive operators are found. Each pair of operands and operators forms a binary expression node, 
 * which is combined into a single abstract syntax tree (AST) node representing the full additive expression.
 *
 * @param parser A pointer to the `Parser` struct, which contains the tokens and necessary 
 *               information for parsing.
 * @return An `AstNode` representing the parsed additive expression.
 *         The returned node is the root of the AST that captures the full expression, with binary operation nodes 
 *         representing individual operations.
 */
AstNode *parse_additive_expr(Parser *parser) {
    int line = at(parser).line;
    int column_start = at(parser).column_start;
    int column_end = at(parser).column_end;
    int position_start = at(parser).position_start;
    int position_end = at(parser).position_end;

    AstNode *left = parse_multiplicative_expr(parser);

    while (at(parser).type == TOKEN_PLUS || at(parser).type == TOKEN_MINUS) {
        char *operator = strdup(at(parser).lexeme);
        eat(parser);
        AstNode *right = parse_multiplicative_expr(parser);

        column_end = at(parser).column_end - 1;
        position_end = at(parser).position_end - 1;

        AstNode *bin_expr = create_ast_node(
            NODE_BINARY_EXPR, 
            create_binary_expr_data(left, right, operator),
            line, 
            column_start, 
            position_start, 
            column_end, 
            position_end
        );

        add_child_to_node(bin_expr, left);
        add_child_to_node(bin_expr, right);

        left = bin_expr;
    }

    return left;
}
