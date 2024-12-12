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
#include "frontend/parser/statements/parse_decorator_stmt.h"
#include "frontend/parser/statements/parse_for_stmt.h"
#include "frontend/parser/statements/parse_extern_stmt.h"
#include "frontend/parser/types/parse_type.h"

AstNode *parse_stmt(Parser *parser) {
    switch (at(parser).type) {
        case TOKEN_PACKAGE: return parse_package_stmt(parser);
        case TOKEN_IMPORT: return parse_import_stmt(parser);
        case TOKEN_FOR: return parse_for_stmt(parser);
        case TOKEN_DEF: return parse_function_declaration_stmt(parser);
        case TOKEN_AT: return parse_decorator_stmt(parser);
        case TOKEN_EXTERN: return parse_extern_stmt(parser);
        default: {
            bool isConst = false;
            // Check if the current token is "const". If so, mark it and consume the token.
            if (at(parser).type == TOKEN_CONST) {
                isConst = true;
                eat(parser); // Consume "const"
            }

            // Attempt to parse a type. If this fails, it's likely an expression.
            char *type = parse_type(parser);

            // If the type is "implicit", this means it's an ambiguous type.
            if (strcmp(type, "implicit") == 0) {
                // If parse_type indicates an implicit type, treat it as a generic expression.
                return parse_expr(parser);
            }

            // Determine if this is a variable declaration by checking if an identifier
            // is followed by an assignment operator.
            bool isDeclaration = false;
            if (at(parser).type == TOKEN_IDENTIFIER && next(parser).type == TOKEN_ASSIGN) {
                isDeclaration = true;
            }

            // If it's determined to be a variable declaration, handle it.
            if (isDeclaration) {
                bool isPtr = false;
                // Check if the variable is a pointer by looking for a "*" token.
                if (at(parser).type == TOKEN_MUL) {
                    isPtr = true;
                    eat(parser); // Consume "*"
                }
                // Parse the declaration with the determined attributes.
                return parse_variable_declaration_stmt(parser, isPtr, isConst, type);
            }

            // If it's not a declaration, treat it as a generic expression.
            return parse_expr(parser);
        }
    }
}
