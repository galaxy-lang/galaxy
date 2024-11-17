#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/lexer.h"
#include "../../include/lexer_error.h"

Token *tokens = NULL;
int tokenCount = 0;
FILE *src = NULL;
char currentChar;
int line = 1;
int col = 1;
int position = 0;
const char *filename = NULL;

void initLexer(FILE *source, const char *file) {
  src = source;
  filename = file;
  currentChar = fgetc(src);
}

void skipWhitespace() {
  while (isspace(currentChar) && currentChar != '\n' && currentChar != '\t') {
    if (currentChar == ' ') {
      col++;
    }
    currentChar = fgetc(src);
    position++;
  }
}

void addToken(TokenType type, const char *lexeme) {
  tokens = realloc(tokens, sizeof(Token) * (tokenCount + 1));
  Token *newToken = &tokens[tokenCount];
  newToken->type = type;
  newToken->lexeme = strdup(lexeme);
  newToken->line = line;
  newToken->column_start = col - strlen(lexeme);
  newToken->column_end = col - 1;
  newToken->position_start = position - strlen(lexeme);
  newToken->position_end = position - 1;
  newToken->filename = filename;
  newToken->message = strdup("");
  tokenCount++;
}

char eat_char() {
  char c = currentChar;
  currentChar = fgetc(src);
  col++;
  position++;
  return c;
}

char pick_char() {
  return currentChar;
}

char pick_next() {
  char next = fgetc(src);
  ungetc(next, src);
  return next;
}

TokenType match_keyword(const char *lexeme) {
  if (strcmp(lexeme, "package") == 0) return TOKEN_PACKAGE;
  if (strcmp(lexeme, "import") == 0) return TOKEN_IMPORT;
  if (strcmp(lexeme, "def") == 0) return TOKEN_DEF;
  if (strcmp(lexeme, "return") == 0) return TOKEN_RETURN;
  if (strcmp(lexeme, "true") == 0 || strcmp(lexeme, "false") == 0) return TOKEN_BOOL;
  return TOKEN_IDENTIFIER;
}

Token getNextToken() {
  skipWhitespace();
  if (pick_char() == '\n') {
    eat_char();
    line++;
    col = 1;
    return (Token){TOKEN_NEWLINE, strdup("\\n"), line - 1, col, col, position, position, filename, ""};
  }
  if (pick_char() == '\t') {
    eat_char();
    col += 4;
    return (Token){TOKEN_TAB, strdup("\\t"), line, col - 4, col - 1, position - 1, position - 1, filename, ""};
  }
  if (isalpha(pick_char()) || pick_char() == '_') {
    char buffer[256];
    int i = 0;
    while (isalnum(pick_char()) || pick_char() == '_') {
      buffer[i++] = eat_char();
    }
    buffer[i] = '\0';
    TokenType type = match_keyword(buffer);
    return (Token){type, strdup(buffer), line, col - i, col - 1, position - i, position - 1, filename, ""};
  }
  if (isdigit(pick_char())) {
    char buffer[256];
    int i = 0;
    int isDecimal = 0;
    while (isdigit(pick_char()) || (pick_char() == '.' && !isDecimal)) {
      if (pick_char() == '.') {
        isDecimal = 1;
      }
      buffer[i++] = eat_char();
    }
    buffer[i] = '\0';
    return (Token){isDecimal ? TOKEN_DECIMAL : TOKEN_INT, strdup(buffer), line, col - i, col - 1, position - i, position - 1, filename, ""};
  }
  if (pick_char() == '"' || pick_char() == '\'') {
    char quote = eat_char();
    char buffer[256];
    int i = 0;
    while (pick_char() != quote && (int)pick_char() != EOF) {
      buffer[i++] = eat_char();
    }
    if ((int)pick_char() == EOF) {
      return (Token){TOKEN_UNKNOWN, strdup("Unclosed string literal"), line, col - 1, col - 1, position, position, filename, "Unclosed string literal"};
    }
    eat_char();
    buffer[i] = '\0';
    return (Token){TOKEN_STRING, strdup(buffer), line, col - i - 1, col - 1, position - i - 1, position - 1, filename, ""};
  }
  switch (pick_char()) {
    case '(':
      eat_char();
      return (Token){TOKEN_LPAREN, strdup("("), line, col - 1, col - 1, position - 1, position - 1, filename, ""};
    case ')':
      eat_char();
      return (Token){TOKEN_RPAREN, strdup(")"), line, col - 1, col - 1, position - 1, position - 1, filename, ""};
    case ',':
      eat_char();
      return (Token){TOKEN_COMMA, strdup(","), line, col - 1, col - 1, position - 1, position - 1, filename, ""};
    case '.':
      eat_char();
      return (Token){TOKEN_DOT, strdup("."), line, col - 1, col - 1, position - 1, position - 1, filename, ""};
    case ':':
      eat_char();
      if (pick_char() == '=') {
        eat_char();
        return (Token){TOKEN_ASSIGN, strdup(":="), line, col - 1, col - 1, position - 1, position - 1, filename, ""};
      }
      return (Token){TOKEN_COLON, strdup(":"), line, col - 1, col - 1, position - 1, position - 1, filename, ""};
    case '+':
      eat_char();
      return (Token){TOKEN_PLUS, strdup("+"), line, col - 1, col - 1, position - 1, position - 1, filename, ""};
    case '>':
      eat_char();
      if (pick_char() == '=') {
        eat_char();
        return (Token){TOKEN_GEQUAL, strdup(">="), line, col - 1, col - 1, position - 1, position - 1, filename, ""};
      }
      return (Token){TOKEN_GT, strdup(">"), line, col - 1, col - 1, position - 1, position - 1, filename, ""};
    case '<':
      eat_char();
      if (pick_char() == '=') {
        eat_char();
        return (Token){TOKEN_LEQUAL, strdup("<="), line, col - 1, col - 1, position - 1, position - 1, filename, ""};
      }
      return (Token){TOKEN_LT, strdup("<"), line, col - 1, col - 1, position - 1, position - 1, filename, ""};
    case '=':
      eat_char();
      if (pick_char() == '=') {
        eat_char();
        return (Token){TOKEN_EQUAL, strdup("=="), line, col - 1, col - 1, position - 1, position - 1, filename, ""};
      }
      return (Token){TOKEN_UNKNOWN, strdup(""), line, col - 1, col - 1, position - 1, position - 1, filename, ""};
    case '-':
      eat_char();
      if (pick_char() == '>') {
        eat_char();
        return (Token){TOKEN_ARROW, strdup("->"), line, col - 2, col - 1, position - 2, position - 1, filename, ""};
      }
      return (Token){TOKEN_MINUS, strdup("-"), line, col - 1, col - 1, position - 1, position - 1, filename, ""};
    case '*':
      eat_char();
      return (Token){TOKEN_MUL, strdup("*"), line, col - 1, col - 1, position - 1, position - 1, filename, ""};
    case '/':
      eat_char();
      return (Token){TOKEN_DIV, strdup("/"), line, col - 1, col - 1, position - 1, position - 1, filename, ""};
    default:
      lexer_error(filename, line, col, position - 1, position, currentChar, "Invalid character");
      eat_char();
      return (Token){TOKEN_UNKNOWN, strdup(""), line, col - 1, col - 1, position - 1, position - 1, filename, ""};
  }
}

int not_eof() {
  return (int)currentChar != EOF;
}

Token *tokenize(FILE *sourceFile, const char *fileName, int *count) {
  initLexer(sourceFile, fileName);
  tokens = NULL;
  tokenCount = 0;
  Token token = getNextToken();

  while (not_eof()) {
    addToken(token.type, token.lexeme);
    token = getNextToken();
  }

  addToken(TOKEN_EOF, "EOF");

 *count = tokenCount;
  return tokens;
}
