#include <string.h>
#include "../../../../include/utils.h"
#include "../../../../include/parser/core.h"
#include "../../../../include/ast/definitions.h"
#include "../../../../include/ast/core.h"
#include "../../../../include/lexer/core.h"
#include "../../../../include/parser/expressions/parse_expr.h"
#include "../../../../include/parser/expressions/parse_primary_expr.h"

AstNode *parse_primary_expr(Parser *parser) {
    Token token = eat(parser);

    switch (token.type) {
        case TOKEN_NUMBER: {
            NumericLiteralNode *numeric_data = malloc(sizeof(NumericLiteralNode));
            if (!numeric_data) {
                fprintf(stderr, "Error: Memory allocation failed for NumericLiteralNode\n");
                exit(EXIT_FAILURE);
            }
            numeric_data->value = strtod(token.lexeme, NULL);

            AstNode *node = create_ast_node(NODE_NUMERIC_LITERAL, numeric_data);
            return node;
        }

        case TOKEN_IDENTIFIER: {
            IdentifierNode *identifier_data = malloc(sizeof(IdentifierNode));
            if (!identifier_data) {
                fprintf(stderr, "Error: Memory allocation failed for IdentifierNode\n");
                exit(EXIT_FAILURE);
            }
            identifier_data->symbol = strdup(token.lexeme);

            AstNode *node = create_ast_node(NODE_IDENTIFIER, identifier_data);
            return node;
        }

        case TOKEN_OPAREN: {
            AstNode *node = parse_expr(parser);
            expect(parser, TOKEN_CPAREN, "Expected closing parenthesis");
            return node;
        }

        default:
            error(parser, "Unexpected token in primary expression");
            return NULL;
    }
}


