#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "frontend/parser/printer/print_indent.h"
#include "frontend/parser/printer/print_ast.h"
#include "frontend/parser/printer/nodes/statements/print_variable.h"

void print_variable(const AstNode *node, int depth, VisitedNodes *visited) {
    VariableNode *variable_data = (VariableNode *)node->data;

    if (!variable_data) {
        print_indent(depth);
        printf("VariableNode is NULL\n");
        return;
    }

    print_indent(depth + 1);
    printf("Variable Name: %s\n", variable_data->name ? variable_data->name : "Unnamed");

    print_indent(depth + 1);
    printf("Is Pointer: %s\n", variable_data->isPtr ? "true" : "false");

    print_indent(depth + 1);
    printf("Is Constant: %s\n", variable_data->isConst ? "true" : "false");

    print_indent(depth + 1);
    printf("Type: %s\n", variable_data->varType);

    print_indent(depth + 1);
    printf("Value:\n");
    if (variable_data->value) {
        print_ast_node(variable_data->value, depth + 2, visited);
    } else {
        print_indent(depth + 2);
        printf("Value is NULL\n");
    }
}
