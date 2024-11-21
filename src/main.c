#include <stdio.h>
#include <stdlib.h>
#include "../include/lexer/core.h"
#include "../include/utils.h"

void freeTokens(Token *tokens, int tokenCount) {
    if (tokens != NULL) {
        for (int i = 0; i < tokenCount; i++) {
            free(tokens[i].lexeme);  
            free(tokens[i].message); 
        }
        free(tokens);  
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

    int count = 0;
    Token *tokens = tokenize(sourceFile, argv[1], &count);

    freeTokens(tokens, count);
    fclose(sourceFile);
    return 0;
}
