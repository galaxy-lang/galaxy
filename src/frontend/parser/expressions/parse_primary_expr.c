#include <string.h>
#include "../../../../include/utils.h"
#include "../../../../include/parser/core.h"
#include "../../../../include/ast/definitions.h"
#include "../../../../include/ast/core.h"
#include "../../../../include/lexer/core.h"
#include "../../../../include/parser/expressions/parse_expr.h"
#include "../../../../include/parser/expressions/parse_primary_expr.h"

/**
 * @brief Parses a primary expression.
 *
 * This function is responsible for parsing the simplest building blocks of expressions, known as primary expressions.
 * These can include:
 * - Numeric literals (e.g., numbers).
 * - Identifiers (e.g., variable names).
 * - Parenthesized expressions (i.e., expressions within parentheses).
 * 
 * The function processes the token at the current parser position and attempts to match it to one of the expected 
 * primary expression types. If a valid primary expression is identified, it creates the corresponding AST node.
 *
 * @param parser A pointer to the `Parser` struct, which contains the tokens to be parsed and other related information.
 * @return An `AstNode` representing the parsed primary expression.
 *         If the token corresponds to a numeric literal, an identifier, or a valid parenthesized expression, 
 *         an AST node is created and returned. Otherwise, an error is raised.
 */
AstNode *parse_primary_expr(Parser *parser) {
    Token token = eat(parser);

    switch (token.type) {
        case TOKEN_NUMBER: {
            int line = at(parser).line;
            int column_start = at(parser).column_start;
            int column_end = at(parser).column_end;
            int position_start = at(parser).position_start;
            int position_end = at(parser).position_end;

            if (token.lexeme == NULL || strlen(token.lexeme) == 0) {
                error(parser, "Invalid number lexeme");
                return NULL;
            }

            NumericLiteralNode *numeric_data = malloc(sizeof(NumericLiteralNode));
            if (!numeric_data) {
                fprintf(stderr, "Error: Memory allocation failed for NumericLiteralNode\n");
                exit(EXIT_FAILURE);
            }

            if (strcmp(token.message, "decimal") == 0) {
                numeric_data->value = strtod(token.lexeme, NULL);
                numeric_data->decimal = true;
            } else {
                numeric_data->value = atoi(token.lexeme);
                numeric_data->decimal = false;
            }            

            AstNode *node = create_ast_node(
                NODE_NUMERIC_LITERAL,
                numeric_data,
                line,
                column_start,
                position_start,
                column_end,
                position_end
            );
            
            return node;
        }

        case TOKEN_IDENTIFIER: {
            int line = at(parser).line;
            int column_start = at(parser).column_start;
            int column_end = at(parser).column_end;
            int position_start = at(parser).position_start;
            int position_end = at(parser).position_end;

            if (token.lexeme == NULL || strlen(token.lexeme) == 0) {
                error(parser, "Invalid identifier lexeme");
                return NULL;
            }

            IdentifierNode *identifier_data = malloc(sizeof(IdentifierNode));
            if (!identifier_data) {
                fprintf(stderr, "Error: Memory allocation failed for IdentifierNode\n");
                exit(EXIT_FAILURE);
            }
            identifier_data->symbol = strdup(token.lexeme);

            AstNode *node = create_ast_node(
                NODE_IDENTIFIER,
                identifier_data,
                line,
                column_start,
                position_start,
                column_end,
                position_end
            );
            return node;
        }

        case TOKEN_OPAREN: {
            AstNode *node = parse_expr(parser);
            if (node == NULL) {
                error(parser, "Failed to parse expression inside parentheses");
                return NULL;
            }

            expect(parser, TOKEN_CPAREN, "Expected closing parenthesis");
            return node;
        }

        default:
            error(parser, "Unexpected token in primary expression");
            return NULL;
    }
}

