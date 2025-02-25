#include "frontend/parser/printer/nodes/expressions/print_array.h"
#include "frontend/parser/printer/visited.h"
#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/print_indent.h"
#include "frontend/parser/printer/print_ast.h"

void print_array(const AstNode *node, int depth, VisitedNodes *visited) {
    ArrayNode *array_data = (ArrayNode *)node->data;

    if (array_data) {
        print_indent(depth + 1);
        printf("Array elements: \n");

        if (array_data->element_count > 0) {
            for (size_t i = 0; i < array_data->element_count; i++) {
                print_indent(depth + 2);
                printf("Element %zu\n", i);

                if (array_data->elements[i]) {
                    print_ast_node(array_data->elements[i], depth + 3, visited);
                } else {
                    print_indent(depth + 3);
                    printf("Element is NULL\n");
                }
            }
        }
    } else {
        print_indent(depth + 1);
        printf("Array data is NULL\n");
    }
}