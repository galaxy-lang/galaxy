#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "frontend/ast/definitions.h"
#include "frontend/parser/expressions/parse_expr.h"
#include "frontend/parser/statements/parse_stmt.h"
#include "frontend/parser/statements/parse_package_stmt.h"
#include "frontend/parser/statements/parse_import_stmt.h"
#include "frontend/parser/statements/parse_variable_declaration_stmt.h"
#include "frontend/parser/statements/parse_function_declaration_stmt.h"
#include "frontend/parser/statements/parse_for_stmt.h"
#include "frontend/parser/types/parse_type.h"
#include "frontend/types.h"

AstNode *parse_stmt(Parser *parser) {
    switch (at(parser).type) {
        case TOKEN_PACKAGE: return parse_package_stmt(parser);
        case TOKEN_IMPORT: return parse_import_stmt(parser);
        case TOKEN_FOR: return parse_for_stmt(parser);
        case TOKEN_DEF: return parse_function_declaration_stmt(parser);
        case TOKEN_AT: return parse_decorator_stmt(parser);
        default: {
            Token *tokens = parser->tokens;
            int idx = parser->index;

            bool isConst = (tokens[idx].type == TOKEN_CONST);
            bool isPtr = (isConst && tokens[idx + 1].type == TOKEN_MUL) || tokens[idx].type == TOKEN_MUL;

            if (isConst) eat(parser); // Consumes "const", if present
            if (isPtr) eat(parser);   // Consumes "*", if present

            Type type = TYPE_IMPLICIT;

            // Verifies if is there an explicit type before the variable name
            if (!isPtr || (isPtr && tokens[idx].type != TOKEN_ASSIGN)) {
                type = parse_type(parser);
            }

            if (next(parser).type == TOKEN_ASSIGN) {
                return parse_variable_declaration_stmt(parser, isPtr, isConst, type);
            }

            // Default case: generic expression
            return parse_expr(parser);
        }
    }
}
