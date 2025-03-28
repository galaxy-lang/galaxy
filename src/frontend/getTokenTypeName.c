#include "frontend/lexer/core.h"
#include "frontend/getTokenTypeName.h"

/**
 * @brief Returns the name of a token type as a string.
 *
 * This function takes a `TokenType` and returns the corresponding string representation of the
 * token type. It is useful for printing information about tokens during lexical analysis.
 *
 * @param type The type of the token (enum `TokenType`).
 * @return A string representing the name of the token type.
 */
const char* getTokenTypeName(TokenType type) {
    switch (type) {
        case TOKEN_AND:          return "AND";
        case TOKEN_ARROW:        return "ARROW";
        case TOKEN_ASSIGN:       return "ASSIGN";
        case TOKEN_AT:           return "AT";
        case TOKEN_BITWISE_AND:  return "BITWISE AND";
        case TOKEN_BITWISE_NOT:  return "BITWISE NOT";
        case TOKEN_BITWISE_OR:   return "BITWISE OR";
        case TOKEN_BITWISE_XOR:  return "BITWISE XOR";
        case TOKEN_CASE:         return "CASE";
        case TOKEN_CBRACE:       return "CLOSE BRACE";
        case TOKEN_CBRACKET:     return "CLOSE BRACKET";
        case TOKEN_COLON:        return "COLON";
        case TOKEN_COMMA:        return "COMMA";
        case TOKEN_CONST:        return "CONST";
        case TOKEN_CPAREN:       return "CLOSED PAREN";
        case TOKEN_DECREMENT:    return "DECREMENT";
        case TOKEN_DEF:          return "DEF";
        case TOKEN_DEFAULT:      return "DEFAULT";
        case TOKEN_DIV:          return "DIVIDE";
        case TOKEN_DOT:          return "DOT";
        case TOKEN_ELIF:         return "ELIF";
        case TOKEN_ELLIPSIS:     return "ELLIPSIS";
        case TOKEN_ELSE:         return "ELSE";
        case TOKEN_END:          return "END";
        case TOKEN_EQUAL:        return "EQUAL";
        case TOKEN_FALSE:        return "FALSE";
        case TOKEN_FOR:          return "FOR";
        case TOKEN_GEQUAL:       return "GREATER OR EQUAL";
        case TOKEN_GT:           return "GREATER THAN";
        case TOKEN_IDENTIFIER:   return "IDENTIFIER";
        case TOKEN_IF:           return "IF";
        case TOKEN_INCREMENT:    return "INCREMENT";
        case TOKEN_IMPORT:       return "IMPORT";
        case TOKEN_IRANGE:       return "INCLUSIVE RANGE";
        case TOKEN_LEQUAL:       return "LESS OR EQUAL";
        case TOKEN_LT:           return "LESS THAN";
        case TOKEN_MINUS:        return "MINUS";
        case TOKEN_MODULUS:      return "MODULUS";
        case TOKEN_MUL:          return "MULTIPLY";
        case TOKEN_NEQUAL:       return "NOT EQUAL";
        case TOKEN_NOT:          return "NOT";
        case TOKEN_NUMBER:       return "NUMBER";
        case TOKEN_OBRACE:       return "OPEN BRACE";
        case TOKEN_OBRACKET:     return "OPEN BRACKET";
        case TOKEN_OPAREN:       return "OPEN PAREN";
        case TOKEN_OR:           return "OR";
        case TOKEN_PACKAGE:      return "PACKAGE";
        case TOKEN_PLUS:         return "PLUS";
        case TOKEN_POWER:        return "POWER";
        case TOKEN_RANGE:        return "RANGE";
        case TOKEN_RETURN:       return "RETURN";
        case TOKEN_SEMICOLON:    return "SEMICOLON";
        case TOKEN_SHIFT_LEFT:   return "SHIFT LEFT";
        case TOKEN_SHIFT_RIGHT:  return "SHIFT RIGHT";
        case TOKEN_STRING:       return "STRING";
        case TOKEN_WHILE:        return "WHILE";
        case TOKEN_SWITCH:       return "SWITCH";
        case TOKEN_TRUE:         return "TRUE";
        case TOKEN_TYPE_I8:      return "TYPE I8";
        case TOKEN_TYPE_I16:     return "TYPE I16";
        case TOKEN_TYPE_I32:     return "TYPE I32";
        case TOKEN_TYPE_I64:     return "TYPE I64";
        case TOKEN_TYPE_I128:    return "TYPE I128";
        case TOKEN_TYPE_U8:      return "TYPE U8";
        case TOKEN_TYPE_U16:     return "TYPE U16";
        case TOKEN_TYPE_U32:     return "TYPE U32";
        case TOKEN_TYPE_U64:     return "TYPE U64";
        case TOKEN_TYPE_U128:    return "TYPE U128";
        case TOKEN_TYPE_ISIZE:   return "TYPE ISIZE";
        case TOKEN_TYPE_USIZE:   return "TYPE USIZE";
        case TOKEN_TYPE_F32:     return "TYPE F32";
        case TOKEN_TYPE_F64:     return "TYPE F64";
        case TOKEN_TYPE_BOOL:    return "TYPE BOOL";
        case TOKEN_TYPE_VOID:    return "TYPE VOID";
        case TOKEN_TYPE_NULL:    return "TYPE NULL";
        case TOKEN_TYPE_UNDEFINED: return "TYPE UNDEFINED";
        case TOKEN_TYPE_ARRAY:   return "TYPE ARRAY";
        case TOKEN_TYPE_SLICE:   return "TYPE SLICE";
        case TOKEN_UNKNOWN:      return "UNKNOWN";
        case TOKEN_EOF:          return "END OF FILE";
        case TOKEN_ENUM:         return "ENUM";
        case TOKEN_SIZEOF:       return "SIZEOF";
        default:                 return "INVALID TOKEN";
    }
}