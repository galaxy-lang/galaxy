#include <stdio.h>
#include <stdlib.h>
#include "../../include/lexer.h"

void freeTokens(Token *tokens, int tokenCount) {
    if (tokens != NULL) {
        for (int i = 0; i < tokenCount; i++) {
            free(tokens[i].lexeme);  // Libera o lexema do token
            free(tokens[i].message); // Libera a mensagem de erro (caso exista)
        }
        free(tokens);  // Libera a memória do array de tokens
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <source_file>\n", argv[0]);
        return 1;
    }

    FILE *sourceFile;
    if (!fopen_safe(sourceFile, argv[1], "r")) {
        fprintf(stderr, "Error opening file '%s'\n", argv[1]);
        return 1;
    }

    int *count;
    Token *tokens = tokenize(sourceFile, argv[1], &count);

    freeTokens(tokens, *count);
    fclose(sourceFile);
    return 0;
}
