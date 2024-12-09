#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "frontend/parser/core.h"
#include "frontend/parser/types/parse_type.h"

char* check_and_append_array(Parser *parser, const char* base_type) {
    if (at(parser).type == TOKEN_OBRACKET) {
        eat(parser);
        if (at(parser).type == TOKEN_CBRACKET) {
            eat(parser);
            size_t len = strlen(base_type) + 3;
            char* array_type = malloc(len);
            snprintf(array_type, len, "%s[]", base_type);
            return array_type;
        }
    }
    return strdup(base_type);
}

char* parse_generic_type(Parser *parser, const char* base_type) {
    eat(parser);
    size_t capacity = 128;
    size_t length = strlen(base_type) + 1;
    char* result = malloc(capacity);
    strcpy(result, base_type);
    strcat(result, "<");

    char* sub_type = parse_type(parser);
    length += strlen(sub_type) + 1;
    if (length >= capacity) {
        capacity *= 2;
        result = realloc(result, capacity);
    }
    strcat(result, sub_type);
    free(sub_type);

    while (at(parser).type == TOKEN_COMMA) {
        eat(parser);
        strcat(result, ",");
        char* next_type = parse_type(parser);
        length += strlen(next_type) + 1;
        if (length >= capacity) {
            capacity *= 2;
            result = realloc(result, capacity);
        }
        strcat(result, next_type);
        free(next_type);
    }

    expect(parser, TOKEN_GT, "Expected \">\".");
    strcat(result, ">");
    return result;
}

char* parse_type(Parser *parser) {
    switch (at(parser).type) {
        case TOKEN_TYPE_INT: 
            eat(parser); 
            return check_and_append_array(parser, "int");

        case TOKEN_TYPE_FLOAT: 
            eat(parser); 
            return check_and_append_array(parser, "float");

        case TOKEN_TYPE_DOUBLE: 
            eat(parser); 
            return check_and_append_array(parser, "double");

        case TOKEN_TYPE_DECIMAL: 
            eat(parser); 
            return check_and_append_array(parser, "decimal");

        case TOKEN_TYPE_STRING: 
            eat(parser); 
            return check_and_append_array(parser, "string");

        case TOKEN_TYPE_VOID: 
            eat(parser); 
            return strdup("void");

        case TOKEN_TYPE_BOOL: 
            eat(parser); 
            return check_and_append_array(parser, "bool");

        case TOKEN_TYPE_LIST: 
            eat(parser); 
            expect(parser, TOKEN_LT, "Expected \"<\".");
            return parse_generic_type(parser, "list");

        case TOKEN_TYPE_TUPLE: 
            eat(parser); 
            expect(parser, TOKEN_LT, "Expected \"<\".");
            return parse_generic_type(parser, "tuple");

        case TOKEN_IDENTIFIER: {
            char* identifier = strdup(eat(parser).lexeme);
            if (at(parser).type == TOKEN_LT) {
                char* generic_type = parse_generic_type(parser, identifier);
                free(identifier);
                return generic_type;
            }
            return check_and_append_array(parser, identifier);
        }

        default:
            return "implicit";
    }
}