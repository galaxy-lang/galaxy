#include "frontend/parser/printer/print_indent.h"

/**
 * @brief Prints the indentation corresponding to the depth of a node.
 *
 * This function inserts spaces to create an indentation effect proportional to the depth 
 * specified. Each depth level adds two spaces.
 *
 * @param depth The depth of the node in the AST.
 */
void print_indent(int depth) {
    for (int i = 0; i < depth; i++) {
        printf("    ");
    }
}