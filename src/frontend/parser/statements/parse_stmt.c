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
<<<<<<< HEAD
#include "frontend/parser/statements/parse_function_declaration_stmt.h"
=======
#include "frontend/parser/statements/parse_function_stmt.h"
>>>>>>> c3d1e8df43473f97bde7c0b1480a3da5228e852b
#include "frontend/parser/types/parse_type.h"
#include "frontend/types.h"

AstNode *parse_stmt(Parser *parser) {
    switch (at(parser).type) {
        case TOKEN_PACKAGE: return parse_package_stmt(parser);
        case TOKEN_IMPORT: return parse_import_stmt(parser);
<<<<<<< HEAD
        case TOKEN_DEF: return parse_function_declaration_stmt(parser);
=======
        case TOKEN_DEF: return parse_function_stmt(parser);
>>>>>>> c3d1e8df43473f97bde7c0b1480a3da5228e852b
        default: {
            Token *tokens = parser->tokens;
            int idx = parser->index;

            // const Type *var := value;
            if (
                tokens[idx].type == TOKEN_CONST
                && tokens[idx + 2].type == TOKEN_MUL
            ) {
                eat(parser);
                Type type = parse_type(parser);
                eat(parser);

                return parse_variable_declaration_stmt(
                    parser,
                    true, // isPtr
                    true, // isConst
                    type
                );
            }

            // const *var := value;
            if (
                tokens[idx].type == TOKEN_CONST
                && tokens[idx + 1].type == TOKEN_MUL
            ) {
                eat(parser);
                eat(parser);

                return parse_variable_declaration_stmt(
                    parser,
                    true, // isPtr
                    true, // isConst
                    TYPE_IMPLICIT
                );
            }

            // *var := value;
            if (
                tokens[idx].type == TOKEN_MUL
                && tokens[idx + 2].type == TOKEN_ASSIGN
            ) {
                eat(parser);

                return parse_variable_declaration_stmt(
                    parser,
                    true, // isPtr
                    false, // isConst
                    TYPE_IMPLICIT
                );
            }

            // Type *var := value;
            if (
                tokens[idx + 1].type == TOKEN_MUL
                && tokens[idx + 3].type == TOKEN_ASSIGN
            ) {
                Type type = parse_type(parser);
                eat(parser);

                return parse_variable_declaration_stmt(
                    parser,
                    true, // isPtr
                    false, // isConst
                    type
                );
            }
            // Type var := value;
            if (
                tokens[idx + 2].type == TOKEN_ASSIGN
            ) {
                Type type = parse_type(parser);

                return parse_variable_declaration_stmt(
                    parser,
                    false, // isPtr
                    false, // isConst
                    type
                );
            }

            // var := value;
            return parse_expr(parser);
        }
    }
}
