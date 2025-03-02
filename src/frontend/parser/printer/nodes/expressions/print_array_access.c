#include "frontend/parser/printer/nodes/expressions/print_array_access.h"
#include "frontend/parser/printer/visited.h"
#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/print_indent.h"
#include "frontend/parser/printer/print_ast.h"

void print_array_access(const AstNode *node, int depth, VisitedNodes *visited) {
    ArrayAccessNode *array_data = (ArrayAccessNode *)node->data;
    
    print_indent(depth + 1);
    printf("Array:\n");
    print_ast_node(array_data->array, depth + 2, visited);

    print_indent(depth + 1);
    printf("Index:\n");
    print_ast_node(array_data->index, depth + 2, visited);
}
