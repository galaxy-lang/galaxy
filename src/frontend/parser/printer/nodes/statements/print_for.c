#include <stdio.h>
#include <stdlib.h>
#include "frontend/types.h"
#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/print_type.h"
#include "frontend/parser/printer/print_indent.h"
#include "frontend/parser/printer/print_ast.h"
#include "frontend/parser/printer/visited.h"
#include "frontend/parser/printer/nodes/statements/print_for.h"

void print_for(const AstNode *node, int depth, VisitedNodes *visited) {
    if (!node || node->kind != NODE_FOR) return;

    ForNode *for_node = (ForNode *)node->data;

    print_indent(depth + 1);
    printf("Iterable:\n");

    print_indent(depth + 2);
    printf("Name: %s\n", for_node->variable);

    print_indent(depth + 2);
    printf("Type: %s\n", print_type(for_node->var_type));

    print_indent(depth + 2);
    printf("Is Pointer: %s\n", for_node->var_isPtr ? "true" : "false");

    print_indent(depth + 2);
    printf("Is Constant: %s\n", for_node->var_isConst ? "true" : "false");

    if (for_node->iterator) {
        print_indent(depth + 1);
        printf("Iterator:\n");

        print_ast_node(for_node->iterator, depth + 2, visited);
    } else {
        print_indent(depth + 1);
        printf("Start:\n");
        print_ast_node(for_node->start, depth + 2, visited);
        print_indent(depth + 1);
        printf("Stop:\n");
        print_ast_node(for_node->stop, depth + 2, visited);

        if (for_node->updater) {
            print_indent(depth + 1);
            printf("Updater:\n");
            print_ast_node(for_node->updater, depth + 2, visited);
        }
    }

    if (for_node->body_count > 0) {
        print_indent(depth + 1);
        printf("Body:\n");
        for (size_t i = 0; i < for_node->body_count; i++){
            print_ast_node(for_node->body[i], depth + 2, visited);
        }
    } else {
        print_indent(depth + 1);
        printf("Body: <NULL>\n");
    }
}