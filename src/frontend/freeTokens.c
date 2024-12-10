#include <stdlib.h>
#include "frontend/lexer/core.h"
#include "frontend/freeTokens.h"

/**
 * @brief Frees the memory allocated for an array of tokens.
 *
 * This function frees both the `lexeme` and `message` strings of each token, as well as the 
 * memory allocated for the array of tokens itself. It helps prevent memory leaks after token 
 * processing is complete.
 *
 * @param tokens The array of tokens to be freed.
 * @param tokenCount The number of tokens in the array.
 */
void freeTokens(Token *tokens, int tokenCount) {
    if (tokens != NULL) {
        for (int i = 0; i < tokenCount; i++) {
            if (tokens[i].lexeme != NULL) {
                free(tokens[i].lexeme);
                tokens[i].lexeme = NULL;
            }
            if (tokens[i].message != NULL) {
                free(tokens[i].message);
                tokens[i].message = NULL;
            }
        }
        free(tokens);
    }
}
