#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"
#include "../../../../include/frontend/ast/definitions.h"
#include "../../../../include/frontend/parser/printer/visited.h"

/**
 * @brief Initializes the VisitedNodes structure.
 *
 * This function allocates memory for the structure and sets its initial capacity.
 *
 * @param visited Pointer to the VisitedNodes structure to initialize.
 */
void init_visited(VisitedNodes *visited) {
    visited->nodes = MALLOC_S(sizeof(AstNode *) * 64);
    visited->count = 0;
    visited->capacity = 64;
}

/**
 * @brief Frees the memory used by the VisitedNodes structure.
 *
 * This function releases all resources associated with the VisitedNodes structure.
 *
 * @param visited Pointer to the VisitedNodes structure to free.
 */
void free_visited(VisitedNodes *visited) {
    free(visited->nodes);
}

/**
 * @brief Checks if a node has already been visited.
 *
 * This function determines whether a specific AST node has already been processed.
 *
 * @param visited Pointer to the VisitedNodes structure.
 * @param node Pointer to the AST node to check.
 * @return True if the node has been visited; otherwise, false.
 */
bool is_visited(VisitedNodes *visited, const AstNode *node) {
    for (size_t i = 0; i < visited->count; i++) {
        if (visited->nodes[i] == node) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Marks a node as visited.
 *
 * This function adds a node to the list of visited nodes, resizing the list if needed.
 *
 * @param visited Pointer to the VisitedNodes structure.
 * @param node Pointer to the AST node to mark as visited.
 */
void mark_visited(VisitedNodes *visited, const AstNode *node) {
    if (visited->count == visited->capacity) {
        visited->capacity *= 2;
        visited->nodes = realloc(visited->nodes, sizeof(AstNode *) * visited->capacity);
    }
    visited->nodes[visited->count++] = node;
}