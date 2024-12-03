#include "frontend/parser/printer/print_type.h"

const char* print_type(Type type) {
    switch (type) {
        case TYPE_INT:      return "int";
        case TYPE_FLOAT:    return "float";
        case TYPE_DOUBLE:   return "double";
        case TYPE_DECIMAL:  return "decimal";
        case TYPE_STRING:   return "string";
        case TYPE_VOID:     return "void";
        case TYPE_BOOL:     return "bool";
        case TYPE_ARRAY:    return "array";
        case TYPE_SLICE:    return "slice";
        case TYPE_TUPLE:    return "tuple";
        case TYPE_OBJECT:   return "object";
        case TYPE_FUNCTION: return "function";
        case TYPE_ABSTRACT: return "abstract";
        case TYPE_IMPLICIT: return "implicit";
        case TYPE_CUSTOM:   return "custom";
        default:            return "unknown";
    }
}
