#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../../../../include/ast/definitions.h"
#include "../../../../include/parser/statements/parse_stmt.h"
#include "../../../../include/parser/expressions/parse_expr.h"

AstNode *parse_stmt(Parser *parser) {
    return parse_expr(parser);
}
