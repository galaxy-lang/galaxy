#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"
#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/visited.h"

/**
 * @brief Initializes the VisitedNodes structure.
 *
 * This function allocates memory for the structure and sets its initial capacity to zero.
 * Memory will be allocated as needed in `mark_visited`.
 *
 * @param visited Pointer to the VisitedNodes structure to initialize.
 */
void init_visited(VisitedNodes *visited) {
    visited->nodes = NULL;  // Nenhuma alocação inicial
    visited->count = 0;
    visited->capacity = 0;
}

/**
 * @brief Frees the memory used by the VisitedNodes structure.
 *
 * This function releases all resources associated with the VisitedNodes structure.
 *
 * @param visited Pointer to the VisitedNodes structure to free.
 */
void free_visited(VisitedNodes *visited) {
    if (visited->nodes != NULL) {
        free(visited->nodes);  // Libera o array de nós
        visited->nodes = NULL; // Evita ponteiros pendurados
    }
    visited->count = 0;
    visited->capacity = 0;
}

/**
 * @brief Checks if a node has already been visited.
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
    // Realoca memória se o limite atual for atingido
    if (visited->count == visited->capacity) {
        size_t new_capacity = (visited->capacity == 0) ? 4 : visited->capacity * 2; // Capacidade inicial ou dobra
        AstNode **new_nodes = realloc(visited->nodes, sizeof(AstNode *) * new_capacity);
        if (new_nodes == NULL) {
            perror("Failed to reallocate memory for VisitedNodes");
            exit(EXIT_FAILURE); // Sai para evitar corrupção de memória
        }
        visited->nodes = new_nodes;
        visited->capacity = new_capacity;
    }
    visited->nodes[visited->count++] = (AstNode *)node; // Adiciona o nó
}
