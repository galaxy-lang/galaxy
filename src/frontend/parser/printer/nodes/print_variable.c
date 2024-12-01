#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/print_indent.h"
#include "frontend/parser/printer/print_ast.h"
#include "frontend/types.h"

void print_variable(const AstNode *node, int depth, VisitedNodes *visited) {
    VariableNode *variable_data = (VariableNode *)node->data;

    if (!variable_data) {
        print_indent(depth);
        printf("VariableNode is NULL\n");
        return;
    }

    // Print the variable name
    print_indent(depth);
    printf("Variable Name: %s\n", variable_data->name ? variable_data->name : "Unnamed");

    // Print whether it's a pointer
    print_indent(depth);
    printf("Is Pointer: %s\n", variable_data->isPtr ? "true" : "false");

    // Print whether it's constant
    print_indent(depth);
    printf("Is Constant: %s\n", variable_data->isConst ? "true" : "false");

    // Print the variable type
    print_indent(depth);
    printf("Type: ");
    switch (variable_data->varType) {
        case TYPE_INT:      printf("int\n"); break;
        case TYPE_FLOAT:    printf("float\n"); break;
        case TYPE_DOUBLE:   printf("double\n"); break;
        case TYPE_DECIMAL:  printf("decimal\n"); break;
        case TYPE_STRING:   printf("string\n"); break;
        case TYPE_VOID:     printf("void\n"); break;
        case TYPE_BOOL:     printf("bool\n"); break;
        case TYPE_ARRAY:    printf("array\n"); break;
        case TYPE_SLICE:    printf("slice\n"); break;
        case TYPE_TUPLE:    printf("tuple\n"); break;
        case TYPE_OBJECT:   printf("object\n"); break;
        case TYPE_FUNCTION: printf("function\n"); break;
        case TYPE_ABSTRACT: printf("abstract\n"); break;
        case TYPE_IMPLICIT: printf("implicit\n"); break;
        case TYPE_CUSTOM:   printf("custom\n"); break;
        default:            printf("unknown\n"); break;
    }

    // Print the value (if present)
    print_indent(depth);
    printf("Value:\n");
    if (variable_data->value) {
        print_ast_node(variable_data->value, depth + 1, visited);
    } else {
        print_indent(depth + 1);
        printf("Value is NULL\n");
    }
}
