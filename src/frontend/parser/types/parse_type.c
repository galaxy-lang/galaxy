#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include "frontend/types.h"
#include "frontend/parser/core.h"
#include "frontend/parser/types/parse_type.h"

Type parse_type(Parser *parser) {
    switch(at(parser).type) {
        case TOKEN_TYPE_INT: eat(parser); return TYPE_INT;
        case TOKEN_TYPE_FLOAT: eat(parser); return TYPE_FLOAT;
        case TOKEN_TYPE_DOUBLE: eat(parser); return TYPE_DOUBLE;
        case TOKEN_TYPE_DECIMAL: eat(parser); return TYPE_DECIMAL;
        case TOKEN_TYPE_STRING: eat(parser); return TYPE_STRING;
        case TOKEN_TYPE_VOID: eat(parser); return TYPE_VOID;
        case TOKEN_TYPE_BOOL: eat(parser); return TYPE_BOOL;
        case TOKEN_IDENTIFIER: {
            char first = eat(parser).lexeme[0];

            if (isupper(first)) {
                return TYPE_CUSTOM;
            } else {
                error(parser, "Custom types must be capitalized.");
                exit(EXIT_FAILURE);
            }
        }
    }
}
