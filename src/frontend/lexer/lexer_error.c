#include <stdio.h>
#include <stdlib.h>
#include "../../../include/utils.h"

/**
 * @brief Reports a lexer error and displays contextual information.
 *
 * This function logs a detailed error message for an unrecognized character in the source file, 
 * showing the filename, line, column, and a caret (^) indicating the position of the issue. 
 * It also highlights the problematic character and exits the program after reporting.
 *
 * @param filename The name of the source file where the error occurred.
 * @param line The line number of the error.
 * @param col The column number of the error.
 * @param positionStart The starting position of the invalid character in the file.
 * @param positionEnd The ending position of the invalid character in the file.
 * @param unexpectedChar The character that caused the error.
 * @param message A descriptive error message.
 */
void lexer_error(const char *filename, int line, int col, int positionStart, int positionEnd, char unexpectedChar, const char *message) {
    fprintf(stderr, "ERROR:\n%s:%d:%d:\n%s: character '%c' is not recognized.\n", filename, line, col, message, unexpectedChar);

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    char buffer[1024];
    int currentLine = 1;
    while (fgets(buffer, sizeof(buffer), file)) {
        if (currentLine == line) {
            fprintf(stderr, "%d | %s\n", line, buffer);

            int spacesBeforeCaret = 0;
            int lineDigits = 0;
            int tempLine = line;

            while (tempLine > 0) {  
                tempLine /= 10;
                lineDigits++;
            }

            spacesBeforeCaret += lineDigits + 2 + 1;  

            for (int i = 0; i < col - 1; i++) {
                if (buffer[i] == '\t') {
                    spacesBeforeCaret += (4 - (spacesBeforeCaret % 4));  
                } else {
                    spacesBeforeCaret += 1;  
                }
            }

            for (int i = 0; i < spacesBeforeCaret; i++) {
                fprintf(stderr, " ");  
            }

            printf("    ");

            for (int i = positionStart; i < positionEnd; i++) {
                fprintf(stderr, "^");  
            }

            fprintf(stderr, "\n");
            break;
        }
        currentLine++;
    }
    fclose(file);
    exit(1);
}
