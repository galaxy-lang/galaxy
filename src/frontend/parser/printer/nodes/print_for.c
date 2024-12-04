#include <stdio.h>
#include <stdlib.h>
#include "frontend/types.h"
#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/print_type.h"
#include "frontend/parser/printer/print_indent.h"
#include "frontend/parser/printer/print_ast.h"
#include "frontend/parser/printer/visited.h"
#include "frontend/parser/printer/nodes/print_for.h"

void print_for(const AstNode *node, int depth, VisitedNodes *visited) {
    if (!node || node->kind != NODE_FOR) return;

    ForNode *for_data = (ForNode *)node->data;
    if (!for_data || !for_data->iterable) {
        printf("Invalid FOR node or missing iterable.\n");
        return;
    }

    IterableNode *iterable_data = (IterableNode *)for_data->iterable->data;
    if (!iterable_data) {
        printf("Invalid IterableNode data.\n");
        return;
    }

    print_indent(depth + 1);
    printf("Iterable:\n");

    print_indent(depth + 2);
    printf("Name: %s\n", iterable_data->iterable);

    print_indent(depth + 2);
    printf("Type: %s\n", print_type(iterable_data->iterable_type));

    print_indent(depth + 2);
    printf("Start: %s\n", iterable_data->start);

    print_indent(depth + 2);
    printf("End: %s\n", iterable_data->end);

    print_indent(depth + 2);
    printf("Action: %s\n", iterable_data->action ? iterable_data->action : "None");

    print_indent(depth + 2);
    printf("Compare: %s\n", iterable_data->compare ? iterable_data->compare : "None");

    print_indent(depth + 1);
    printf("Body:\n");

    if (for_data->body_count == 0) {
        print_indent(depth + 2);
        printf("No body statements.\n");
    } else {
        for (int i = 0; i < for_data->body_count; i++) {
            print_ast_node(for_data->body[i], depth + 2, visited);
        }
    }
}