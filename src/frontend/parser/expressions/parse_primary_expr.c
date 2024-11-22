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
        case TOKEN_NUMBER:
            {
                AstNode *node = create_ast_node(NODE_NUMERIC_LITERAL, NULL);
                node->data = malloc(sizeof(double));
                *((double *)node->data) = strtod(token.lexeme, NULL);
                return node;
            }

        case TOKEN_IDENTIFIER:
            {
                AstNode *node = create_ast_node(NODE_IDENTIFIER, NULL);
                node->data = strdup(token.lexeme);  // Store the symbol string
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

