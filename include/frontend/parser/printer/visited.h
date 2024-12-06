#ifndef VISITED_H
#define VISITED_H

#include "../../ast/definitions.h"

typedef struct VisitedNodes {
    AstNode **nodes;       /**< List of pointers to visited nodes. */
    size_t count;          /**< Number of nodes currently stored. */
    size_t capacity;       /**< Maximum capacity of the list before resizing. */
} VisitedNodes;

void init_visited(VisitedNodes *visited);
bool is_visited(VisitedNodes *visited, const AstNode *node);
void mark_visited(VisitedNodes *visited, const AstNode *node);
void free_visited(VisitedNodes *visited);

#endif // VISITED_H