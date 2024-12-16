#include "frontend/parser/printer/nodes/statements/print_if.h"
#include "frontend/parser/printer/visited.h"
#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/print_indent.h"
#include "frontend/parser/printer/print_ast.h"

void print_if(const AstNode *node, int depth, VisitedNodes *visited) {
    if (!node || node->kind != NODE_IF) {
        fprintf(stderr, "Error: Node is not an IfNode\n");
        return;
    } 
    IfNode* if_data = (IfNode*)node->data;

    print_indent(depth + 1);
    printf("Condition: \n");

    print_ast_node(if_data->condition, depth + 2, visited);

    print_indent(depth + 1);
    printf("Consequent: \n");

    for(int i = 0; i < if_data->consequent_count; i++) {
        print_ast_node(if_data->consequent[i], depth + 2, visited);
    }

    if (if_data->alternate) {
        print_indent(depth + 1);
        printf("Alternate : \n");
    
        for (int i = 0; i < if_data->alternate_count; i++) {
            print_ast_node(if_data->alternate[i], depth + 2, visited);
        }
    }
}
