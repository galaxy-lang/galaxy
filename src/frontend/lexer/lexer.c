#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../include/lexer/core.h"
#include "../../../include/lexer/error.h"
#include "../../../include/utils.h"

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


void skipWhiteSpace() 
{

	while (isspace(currentChar))
	{
		if (currentChar == ' ')
		{
			col++;
		} 

		currentChar = fgetc(src);
		position++;
	}

}


char *safe_strdup(const char *str) {
    if (!str) return strdup("");
    char *dup = strdup(str);
    if (!dup) {
        fprintf(stderr, "Error duplicating string\n");
        exit(EXIT_FAILURE);
    }
    return dup;
}

void addToken(TokenType type, const char *lexeme, int line, int col_s, int col_e, int pos_s, int pos_e, const char *filename, char *message) {
    tokens = realloc(tokens, sizeof(Token) * (tokenCount + 1));
    Token *newToken = &tokens[tokenCount];
    newToken->type = type;
    newToken->lexeme = safe_strdup(lexeme);
    newToken->line = line;
    newToken->column_start = col_s;
    newToken->column_end = col_e;
    newToken->position_start = pos_s;
    newToken->position_end = pos_e;
    newToken->filename = filename;
    newToken->message = safe_strdup(message);
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
    if (strcmp(lexeme, "if") == 0) return TOKEN_IF;
    if (strcmp(lexeme, "else") == 0) return TOKEN_ELSE;
    if (strcmp(lexeme, "elif") == 0) return TOKEN_ELIF;
    if (strcmp(lexeme, "switch") == 0) return TOKEN_SWITCH;
    if (strcmp(lexeme, "case") == 0) return TOKEN_CASE;
    if (strcmp(lexeme, "default") == 0) return TOKEN_DEFAULT;
    if (strcmp(lexeme, "package") == 0) return TOKEN_PACKAGE;
    if (strcmp(lexeme, "import") == 0) return TOKEN_IMPORT;
    if (strcmp(lexeme, "def") == 0) return TOKEN_DEF;
    if (strcmp(lexeme, "return") == 0) return TOKEN_RETURN;
    if (strcmp(lexeme, "true") == 0 || strcmp(lexeme, "false") == 0) return TOKEN_BOOL;
    return TOKEN_IDENTIFIER;
}

TokenType match_operator(char op) {
    switch (op) {
        case '+': return TOKEN_PLUS;
        case '/': return TOKEN_DIV;
        case '%': return TOKEN_MODULUS;
        case '^': return TOKEN_POWER;
        case '<': return TOKEN_LT;
	case '...': return TOKEN_ELLIPSIS;
	case '*': return TOKEN_ASTERISK;
        default: return TOKEN_OPERATOR;
    }
}

Token getNextToken() {

	skipWhiteSpace();	

	if (pick_char() == '\n')
	{
		eat_char();
		line++;
		col = 1;
	}

    if (isalpha(pick_char()) || pick_char() == '_') {
        char buffer[256];
        int i = 0;
        while (isalnum(pick_char()) || pick_char() == '_') {
            if (i >= (int)sizeof(buffer) - 1) {
                lexer_error(filename, line, col, position, position, currentChar, "Identifier too long");
                break;
            }
            buffer[i++] = eat_char();
        }
        buffer[i] = '\0';
        TokenType type = match_keyword(buffer);
        return (Token){type, safe_strdup(buffer), line, col - i, col, position - i, position, filename, ""};
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
        return (Token){isDecimal ? TOKEN_DECIMAL : TOKEN_INT, safe_strdup(buffer), line, col - i, col, position - i, position, filename, ""};
    }
    if (pick_char() == '"' || pick_char() == '\'') {
        char quote = eat_char();
        char buffer[256];
        int i = 0;
        while (pick_char() != quote && (int)pick_char() != EOF) {
            buffer[i++] = eat_char();
        }
        if ((int)pick_char() == EOF) {
            return (Token){TOKEN_UNKNOWN, safe_strdup("Unclosed string literal"), line, col - 1, col - 1, position, position, filename, "Unclosed string literal"};
        }
        eat_char();
        buffer[i] = '\0';
        return (Token){TOKEN_STRING, safe_strdup(buffer), line, col - i, col, position - i, position, filename, ""};
    }
    switch (pick_char()) {
        case '(':
            eat_char();
            return (Token){TOKEN_LPAREN, safe_strdup("("), line, col - 1, col, position - 1, position, filename, ""};

        case ')':
            eat_char();
            return (Token){TOKEN_RPAREN, safe_strdup(")"), line, col - 1, col, position - 1, position, filename, ""};

        case ',':
            eat_char();
            return (Token){TOKEN_COMMA, safe_strdup(","), line, col - 1, col, position - 1, position, filename, ""};

        case '.':
            eat_char();
            return (Token){TOKEN_DOT, safe_strdup("."), line, col - 1, col, position - 1, position, filename, ""};

        case ':':
            eat_char();
            if (pick_char() == '=') {
                eat_char();
                return (Token){TOKEN_ASSIGN, safe_strdup(":="), line, col - 2, col, position - 2, position, filename, ""};
            }
            return (Token){TOKEN_COLON, safe_strdup(":"), line, col - 1, col, position - 1, position, filename, ""};

        case '=':
            eat_char();
            return (Token){TOKEN_EQUAL, safe_strdup("="), line, col - 1, col, position - 1, position, filename, ""};

        case '<':
            eat_char();
            if (pick_char() == '=') {
                eat_char();
                return (Token){TOKEN_LEQUAL, safe_strdup("<="), line, col - 2, col, position - 2, position, filename, ""};
            }
            return (Token){TOKEN_LT, safe_strdup("<"), line, col - 1, col, position - 1, position, filename, ""};

        case '>':
            eat_char();
            if (pick_char() == '=') {
                eat_char();
                return (Token){TOKEN_GEQUAL, safe_strdup(">="), line, col - 2, col, position - 2, position, filename, ""};
            }
        return (Token){TOKEN_GT, safe_strdup(">"), line, col - 1, col, position - 1, position, filename, ""};

        case '-':
            eat_char();
            if (pick_char() == '>') {
                eat_char();
                return (Token){TOKEN_ARROW, safe_strdup("->"), line, col - 2, col, position - 2, position, filename, ""};
            }
            return (Token){TOKEN_MINUS, safe_strdup("-"), line, col - 1, col, position - 1, position, filename, ""};


        case '+':
        case '/':
        case '%':
        case '^':
	case '*':
	case '...':

            eat_char();
            return (Token){match_operator(currentChar), safe_strdup(&currentChar), line, col - 1, col, position - 1, position, filename, ""};
	case EOF:
	 break;
	default:
            lexer_error(filename, line, col, position - 1, position, currentChar, "Invalid character");
            eat_char();
            return (Token){TOKEN_UNKNOWN, safe_strdup(""), line, col - 1, col - 1, position - 1, position - 1, filename, ""};
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
        addToken(
            token.type,
            token.lexeme,
            token.line,
            token.column_start,
            token.column_end,
            token.position_start,
            token.position_end,
            token.filename,
            token.message
        );
        token = getNextToken();
    }

    addToken(
        TOKEN_EOF,
        "EOF",
        line,
        col - 1,
        col,
        position - 1,
        position,
        filename,
        ""
    );

    *count = tokenCount;
    return tokens;
}
