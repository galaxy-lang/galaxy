#include <stdio.h>
#include <stdlib.h>

void lexer_error(const char *filename, int line, int col, int positionStart, int positionEnd, char unexpectedChar, const char *message) {
    fprintf(stderr, "%s:%d:%d: Error: %s: character '%c' is not recognized.\n", filename, line, col, message, unexpectedChar);

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    char buffer[1024];
    int currentLine = 1;
    while (fgets(buffer, sizeof(buffer), file)) {
        if (currentLine == line) {
            fprintf(stderr, "%d | %s", line, buffer);

            int spacesBeforeCaret = 0;
            int lineDigits = 0;
            int tempLine = line;

            while (tempLine > 0) {  // Conta o número de dígitos do número da linha
                tempLine /= 10;
                lineDigits++;
            }

            spacesBeforeCaret += lineDigits + 2 + 1;  // Número de dígitos da linha + 2 espaços e 1 para o '|'

            for (int i = 0; i < col - 1; i++) {
                if (buffer[i] == '\t') {
                    spacesBeforeCaret += (4 - (spacesBeforeCaret % 4));  // Conta os espaços equivalentes a uma tabulação
                } else {
                    spacesBeforeCaret += 1;  // Conta os espaços normais
                }
            }

            for (int i = 0; i < spacesBeforeCaret; i++) {
                fprintf(stderr, " ");  // Imprime os espaços necessários
            }

            printf("    ");

            for (int i = positionStart; i < positionEnd; i++) {
                fprintf(stderr, "^");  // Imprime os carets (^) que indicam o erro
            }

            fprintf(stderr, "\n");
            break;
        }
        currentLine++;
    }
    fclose(file);
    exit(1);
}
