#include <stdio.h>
#include <stdlib.h>
#include "frontend/lexer/core.h"
#include "frontend/lexer/freeTokens.h"
#include "frontend/getTokenTypeName.h"
#include "utils.h"
#include "frontend/freeTokens.h"

/**
 * @brief The main entry point of the program for lexical analysis.
 *
 * This program reads a source file specified by the user, performs lexical analysis by tokenizing the contents of the file, 
 * and prints out the token details such as their type, position (line and column), and lexeme.
 *
 * The program expects the source file to be passed as an argument to the executable. If the file is successfully opened,
 * the program will tokenize the contents of the file and display each token's information. Afterward, it frees the allocated 
 * memory for the tokens and closes the file.
 *
 * @param argc The number of command-line arguments passed to the program.
 * @param argv The array of command-line arguments, where the second argument (argv[1]) is the path to the source file.
 * @return Returns 0 on successful execution, or 1 if the program fails to open the source file or if the file argument is missing.
 */
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

    printf("Lexical Analysis:\n");
    printf("-----------------\n");

    for (int i = 0; i < count; i++) {
        Token token = tokens[i];
        printf("Line %d, Column %d-%d: %s \"%s\"\n",
               token.line,
               token.column_start,
               token.column_end,
               getTokenTypeName(token.type),
               token.lexeme);
    }

    freeTokens(tokens, count);
    fclose(sourceFile);
    return 0;
}
