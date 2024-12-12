#include <stdio.h>
#include <stdlib.h>
#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/print_indent.h"
#include "frontend/parser/printer/print_ast.h"
#include "frontend/parser/printer/visited.h"
#include "frontend/parser/printer/nodes/statements/print_extern.h"

void print_extern(const AstNode *node, int depth) {
    ExternNode *extern_data = (ExternNode *)node->data;

    print_indent(depth + 1);
    printf("Extern Type: %s\n", extern_data->extern_type);

    print_indent(depth + 1);
    printf("Type: %s\n", extern_data->type);

    print_indent(depth + 1);
    printf("Identifier: %s\n", extern_data->identifier);

    if (extern_data->args) {
        print_indent(depth + 1);
        printf("Arguments:\n");
        for (int i = 0; i < extern_data->arg_count; i++) {
            print_indent(depth + 2);
            printf("%s\n", extern_data->args[i]);
        }
    }
}