#ifndef LEXER_ERROR_H
#define LEXER_ERROR_H

#include <stdio.h>
#include <stdlib.h>

void lexer_error(const char *filename, int line, int col, int positionStart, int positionEnd, char unexpectedChar, const char *message);

#endif // LEXER_ERROR_H
