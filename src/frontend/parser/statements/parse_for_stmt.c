#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "frontend/parser/statements/parse_for_stmt.h"
#include "frontend/parser/statements/parse_stmt.h"
#include "frontend/parser/types/parse_type.h"
#include "frontend/parser/core.h"
#include "frontend/types.h"
#include "utils.h"

AstNode *parse_for_stmt(Parser *parser) {
	int line = at(parser).line;
	int column_start = at(parser).column_start;
	int position_start = at(parser).position_start;

	eat(parser);
 	expect(parser, TOKEN_OPAREN, "Expected \"(\".");

	Type iterable_type = parse_type(parser);
	char *iterable = expect(parser, TOKEN_IDENTIFIER, "Expected identifier").lexeme;

	char *start, *end, *action = NULL, *compare = NULL;

	if (at(parser).type == TOKEN_ASSIGN) {
		eat(parser);
		start = eat(parser).lexeme;
		expect(parser, TOKEN_SEMICOLON, "Expected \";\".");

		if (strcmp(iterable, eat(parser).lexeme) != 0) {
			error(parser, "Iterable is different from the operand.");
			exit(EXIT_FAILURE);
		}

		compare = eat(parser).lexeme;
		end = eat(parser).lexeme;
		expect(parser, TOKEN_SEMICOLON, "Expected \";\".");

		if (strcmp(iterable, eat(parser).lexeme) != 0) {
			error(parser, "Unexpected iterable.");
			exit(EXIT_FAILURE);
		}

		action = eat(parser).lexeme;
	} else if (at(parser).type == TOKEN_COLON) {
		eat(parser);

		if (next(parser).type == TOKEN_RANGE || next(parser).type == TOKEN_IRANGE) {
			start = eat(parser).lexeme;
			eat(parser);
			end = eat(parser).lexeme;
		} else {
			start = at(parser).lexeme;
			end = at(parser).lexeme;
			eat(parser);
		}
	} else {
		error(parser, "Expected \":=\" or \":\".");
		exit(EXIT_FAILURE);
	}

	expect(parser, TOKEN_CPAREN, "Expected \")\".");
	expect(parser, TOKEN_COLON, "Expected \":\".");

	AstNode *iterable_data = create_iterable_data(
		iterable,
		iterable_type,
		start,
		end,
		action,
		compare
	);

	ForNode *for_data = MALLOC_S(sizeof(ForNode));
	for_data->iterable = iterable_data;
	for_data->body = NULL;
	for_data->body_count = 0;

	while (not_eof(parser) && at(parser).type != TOKEN_END) {
		for_data->body = realloc(
			for_data->body,
			sizeof(AstNode *) * (for_data->body_count + 1)
		);

		if (!for_data->body) {
			fprintf(stderr, "Error: Failed to reallocate to for stmt.");
			exit(EXIT_FAILURE);
		}

		for_data->body[for_data->body_count] = parse_stmt(parser);
		for_data->body_count++;
	}

	expect(parser, TOKEN_END, "Expected end.");

	int column_end = at(parser).column_end - 1;
	int position_end = at(parser).position_end - 1;

	expect(parser, TOKEN_SEMICOLON, "Expected \";\".");

	AstNode *for_node = create_ast_node(
		NODE_FOR,
		for_data,
		line,
		column_start,
		position_start,
		column_end,
		position_end
	);

	return for_node;
}
