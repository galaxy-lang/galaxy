#include "frontend/parser/printer/nodes/print_function.h"
#include "frontend/parser/printer/visited.h"
#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/print_indent.h"
#include "frontend/parser/printer/print_ast.h"
#include "frontend/parser/printer/print_type.h"

void print_function(const AstNode *node, int depth, VisitedNodes *visited) {
    if (!node || node->kind != NODE_FUNCTION) return;

    FunctionNode *func_data = (FunctionNode *)node->data;

    print_indent(depth + 1);
    printf("Function:\n");

    print_indent(depth + 2);
    printf("Name: %s\n", func_data->name);

    print_indent(depth + 2);
    printf("Return Type: ");
    print_type(func_data->type);
    printf("\n");

    print_indent(depth + 2);
    printf("Is Constant: %s\n", func_data->isConst ? "true" : "false");

    print_indent(depth + 2);
    printf("Is Pointer: %s\n", func_data->isPtr ? "true" : "false");

    print_indent(depth + 2);
    printf("Parameters:\n");

    ParametersNode *params = func_data->parameters;
    if (params->parameter_count == 0) {
        print_indent(depth + 3);
        printf("No parameters.\n");
    } else {
        for (int i = 0; i < params->parameter_count; i++) {
            AstNode *param_node = params->parameters[i];
            ParameterNode *param_data = (ParameterNode *)param_node->data;

            print_indent(depth + 3);
            printf("Parameter Name: %s\n", param_data->name);

            print_indent(depth + 4);
            printf("Type: ");
            print_type(param_data->type);
            printf("\n");

            print_indent(depth + 4);
            printf("Is Constant: %s\n", param_data->isConst ? "true" : "false");

            print_indent(depth + 4);
            printf("Is Pointer: %s\n", param_data->isPtr ? "true" : "false");
        }
    }

    print_indent(depth + 2);
    printf("Body:\n");
    for (int i = 0; i < func_data->body_count; i++) {
        print_ast_node(func_data->body[i], depth + 3, visited);
    }
}
