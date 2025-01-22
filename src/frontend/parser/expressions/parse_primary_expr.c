#include <string.h>
#include "utils.h"
#include <stdbool.h>
#include "frontend/parser/core.h"
#include "frontend/ast/definitions.h"
#include "frontend/ast/core.h"
#include "frontend/lexer/core.h"
#include "frontend/parser/expressions/parse_expr.h"
#include "frontend/parser/expressions/parse_primary_expr.h"

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
    Token token = consume_token(parser);

    int line = current_token(parser).line;
    int column_start = current_token(parser).column_start;
    int column_end = current_token(parser).column_end;
    int position_start = current_token(parser).position_start;
    int position_end = current_token(parser).position_end;

    switch (token.type) {
        case TOKEN_NUMBER: {
            NumericLiteralNode *numeric_data = MALLOC_S(sizeof(NumericLiteralNode));

            if (strcmp(token.message, "decimal") == 0) {
                numeric_data->value = strtod(token.lexeme, NULL);
                numeric_data->decimal = true;
		numeric_data->ty = "float";
            } else {
                numeric_data->value = atoi(token.lexeme);
                numeric_data->decimal = false;
		numeric->data->ty = "int";
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

	    node->tocheck = true;
            return node;
        }

        case TOKEN_IDENTIFIER: {
            if (token.lexeme == NULL || strlen(token.lexeme) == 0) {
                error(parser, "Invalid identifier lexeme");
                return NULL;
            }

            IdentifierNode *identifier_data = MALLOC_S(sizeof(IdentifierNode));

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

        case TOKEN_STRING: {
            StringNode *string_data = MALLOC_S(sizeof(StringNode));

            string_data->string = strdup(token.lexeme);

            AstNode *node = create_ast_node(
                NODE_STRING,
                string_data,
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

            expect(parser, TOKEN_CPAREN, "Expected \")\".");
            return node;
        }

        case TOKEN_FALSE:
        case TOKEN_TRUE: {
            char *value = consume_token(parser).lexeme;
            column_end = current_token(parser).column_end - 1;
            position_end = current_token(parser).position_end - 1;

            BooleanLiteralNode *boolean_data = MALLOC_S(sizeof(BooleanLiteralNode));
            boolean_data->value = value;
	    boolean_data->ty = "bool";
            AstNode *node = create_ast_node(
                NODE_BOOLEAN_LITERAL,
                boolean_data,
                line,
                column_start,
                position_start,
                column_end,
                position_end
            );

	    node->tocheck = true;
            return node;
        }

        case TOKEN_RETURN: {
            AstNode *value = parse_expr(parser);

            expect(parser, TOKEN_SEMICOLON, "Expected \";\".");

            column_end = current_token(parser).column_end - 1;
            position_end = current_token(parser).position_end - 1;

            ReturnNode *return_data = MALLOC_S(sizeof(ReturnNode));
            return_data->value = value;

            AstNode *node = create_ast_node(
                NODE_RETURN,
                return_data,
                line,
                column_start,
                position_start,
                column_end,
                position_end
            );

            return node;
        }

        default:
            error(parser, "Unexpected token in primary expression");
            exit(EXIT_FAILURE);
            return NULL;
    }
}
