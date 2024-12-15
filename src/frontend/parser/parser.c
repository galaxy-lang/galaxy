#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "utils.h"
#include "frontend/ast/definitions.h"
#include "frontend/ast/core.h"
#include "frontend/lexer/core.h"
#include "frontend/parser/statements/parse_stmt.h"
#include "frontend/parser/printer/print_ast.h"
#include "frontend/parser/core.h"

#define MAX_LINE_LENGTH 1024

// Error colors
#define ANSI_RESET   "\x1b[0m"
#define ANSI_RED     "\x1b[31m"

/**
 * @brief Initializes a new parser instance.
 *
 * Creates and returns a new `Parser` object with default values for its fields, such as
 * `tokens`, `token_count`, `errstate`, `lines`, `line_count`, and `index`.
 *
 * @return A new `Parser` instance with initialized fields.
 */
Parser parser_new() {
    Parser parser;
    parser.tokens = NULL;
    parser.token_count = 0;
    parser.errstate = false;
    parser.lines = NULL;
    parser.line_count = 0;
    parser.index = 0;
    return parser;
}

/**
 * @brief Reads all lines from a file and stores them in the parser.
 *
 * This function reads the content of the specified file line by line, storing each line
 * in the `lines` field of the parser. The total number of lines is stored in `line_count`.
 * If the file cannot be opened or memory allocation fails, the program exits with an error.
 *
 * @param filename The name of the file to read.
 * @param parser The parser instance to store the lines in.
 */
void read_lines(const char *filename, Parser *parser) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char **lines = NULL;
    char buffer[MAX_LINE_LENGTH];
    int line_count = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        lines = realloc(lines, sizeof(char *) * (line_count + 1));
        if (!lines) {
            fprintf(stderr, "Error: Memory allocation failed for lines\n");
            exit(EXIT_FAILURE);
        }
        lines[line_count] = strdup(buffer);
        if (!lines[line_count]) {
            fprintf(stderr, "Error: Memory allocation failed for line content\n");
            exit(EXIT_FAILURE);
        }
        line_count++;
    }

    fclose(file);

    parser->lines = lines;
    parser->line_count = line_count;
    free(lines);
}

/**
 * @brief Checks if the parser has reached the end of the tokens.
 *
 * This function determines whether the current token is of type `TOKEN_EOF`, indicating
 * the end of the token stream.
 *
 * @param parser The parser instance.
 * @return `true` if the current token is not `TOKEN_EOF`, otherwise `false`.
 */
bool not_eof(Parser *parser) {
    return parser->tokens[parser->index].type != TOKEN_EOF;
}

/**
 * @brief Retrieves the current token without advancing the parser.
 *
 * Returns the token at the current index in the parser's token list.
 *
 * @param parser The parser instance.
 * @return The current token.
 */
Token current_token(Parser *parser) {
    return parser->tokens[parser->index];
}

/**
 * @brief Retrieves the current token and advances the parser index.
 *
 * This function returns the token at the current index and increments the parser's
 * index to point to the next token.
 *
 * @param parser The parser instance.
 * @return The current token before advancing.
 */
Token consume_token(Parser *parser) {
    return parser->tokens[parser->index++];
}

/**
 * @brief Retrieves the next token without advancing the parser.
 *
 * Returns the token after the current one, or the current token if the index is at the
 * last token in the list.
 *
 * @param parser The parser instance.
 * @return The next token in the stream.
 */
Token next_token(Parser *parser) {
    if (parser->index + 1 >= parser->token_count) {
        return current_token(parser);
    }
    return parser->tokens[parser->index + 1];
}

/**
 * @brief Reports a parsing error and logs contextual information.
 *
 * This function reports an error message based on the current token in the parser.
 * It logs the error location (file, line, and column) and highlights the problematic
 * area in the source code. The parser enters an error state, and the current token
 * is consumed to prevent further processing of the same token.
 *
 * @param parser The parser instance.
 * @param message The error message to display.
 */
void error(Parser *parser, const char *message) {
    Token token = current_token(parser);
    int line = token.line;
    int column_start = token.column_start;
    int column_end = token.column_end;

    fprintf(stderr, "%s:%d:%d: ", token.filename, line, column_start);
    fprintf(stderr, ANSI_RED);
    fprintf(stderr, "ERROR");
    fprintf(stderr, ANSI_RESET);
    fprintf(stderr, ": ");
    fprintf(stderr, "%s\n", message);

    if (parser->lines && line - 1 < parser->line_count) {
    char *line_content = parser->lines[line - 1];
    fprintf(stderr, " %d |   %s\n", line, line_content);

    int line_width = snprintf(NULL, 0, "%d", line);
    int length_of_digit = floor(log10(abs(line))) + 1;
    
    for (int i = 0; i < length_of_digit; i++) {
        fprintf(stderr, " ");
    }
    fprintf(stderr, "  |");

    fprintf(stderr, "%*s", line_width + 6 - length_of_digit - 3, "");

    for (int i = 0; i < column_start - 1; i++) {
        fprintf(stderr, " ");
    }

    fprintf(stderr, ANSI_RED);
    for (int i = column_start; i < column_end; i++) {
        fprintf(stderr, "^");
    }
    fprintf(stderr, ANSI_RESET);
    fprintf(stderr, "\n\n");
}


    parser->errstate = true;
    consume_token(parser);
}

/**
 * @brief Ensures the next token matches the expected type.
 *
 * Checks if the next token is of the specified `expected_type`. If it matches, the token
 * is consumed and returned. If it does not match, an error message is logged, and the
 * parser enters an error state. If the token is `TOKEN_UNKNOWN`, an additional message
 * is logged to indicate an invalid token.
 *
 * @param parser The parser instance.
 * @param expected_type The expected token type.
 * @param err The error message to display if the token does not match.
 * @return The consumed token, or the token at the current index if it is `TOKEN_EOF`.
 */
Token expect(Parser *parser, TokenType expected_type, const char *err) {
    Token prev = consume_token(parser);

    if (prev.type == TOKEN_EOF) {
        error(parser, err);
        error(parser, "Reached End of File");
        exit(EXIT_FAILURE);
    }

    if (prev.type != expected_type) {
        if (prev.type == TOKEN_UNKNOWN) {
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg), "Expected '%d' token, but got a bad token: '%s'", expected_type, prev.lexeme);
            error(parser, error_msg);
        } else {
            error(parser, err);
        }
    }

    return prev;
}

/**
 * @brief Parses a stream of tokens into an abstract syntax tree (AST).
 *
 * This function initializes the parser with the given token stream and sets up the
 * program node of the AST. It reads all lines of the source file for error reporting
 * and iterates through the tokens to parse statements into AST nodes.
 *
 * Each parsed statement node is added to the `statements` array of the program node.
 * If memory allocation fails during this process, the program exits with an error.
 *
 * @param parser The parser instance.
 * @param tokens The array of tokens to parse.
 * @param token_count The total number of tokens in the array.
 * @return The root node of the AST (a program node).
 */
AstNode *produce_ast(Parser *parser, Token *tokens, int token_count) {
    parser->tokens = tokens;
    parser->token_count = token_count;
    parser->index = 0;

    read_lines(tokens->filename, parser);

    ProgramNode *program_data = MALLOC_S(sizeof(ProgramNode));
    program_data->statements = NULL;
    program_data->statement_count = 0;

    AstNode *program_node = create_ast_node(NODE_PROGRAM, program_data, 0, 0, 0, 0, 0);

    while (not_eof(parser)) {
        AstNode *stmt_node = parse_stmt(parser);

        if (stmt_node) {
            program_data->statements = REALLOC_S(
                program_data->statements,
                sizeof(AstNode *) * (program_data->statement_count + 1)
            );

            program_data->statements[program_data->statement_count++] = stmt_node;
        }
    }

    return program_node;
}

