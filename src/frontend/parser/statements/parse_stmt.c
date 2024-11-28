#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../../../../include/frontend/ast/definitions.h"
#include "../../../../include/frontend/parser/expressions/parse_expr.h"
#include "../../../../include/frontend/parser/statements/parse_stmt.h"
#include "../../../../include/frontend/parser/statements/parse_package_stmt.h"
#include "../../../../include/frontend/parser/statements/parse_import_stmt.h"

AstNode *parse_stmt(Parser *parser) {
    switch (at(parser).type) {
        case TOKEN_PACKAGE: return parse_package_stmt(parser);
        case TOKEN_IMPORT: return parse_import_stmt(parser);
        default: return parse_expr(parser);
    }
}
