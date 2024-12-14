#include <stdlib.h>
#include <stdbool.h>
#include "frontend/parser/statements/parse_function_declaration_stmt.h"
#include "frontend/parser/statements/parse_stmt.h"
#include "frontend/parser/types/parse_type.h"
#include "frontend/parser/core.h"
#include "utils.h"

AstNode *parse_function_declaration_stmt(Parser *parser) {
	int line = current_token(parser).line;
	int column_start = current_token(parser).column_start;
	int position_start = current_token(parser).position_start;

	consume_token(parser);

	char *name = expect(parser, TOKEN_IDENTIFIER, "Expected identifier").lexeme;

 	expect(parser, TOKEN_OPAREN, "Expected \"(\".");

	ParametersNode *parameters_data = MALLOC_S(sizeof(ParametersNode));
	parameters_data->parameters = NULL;
	parameters_data->parameter_count = 0;

	while (not_eof(parser) && current_token(parser).type != TOKEN_CPAREN) {
		bool isConst = false;
		bool isPtr = false;

		int line_param = current_token(parser).line;
		int column_start_param = current_token(parser).column_start;
		int position_start_param = current_token(parser).position_start;

 		if (current_token(parser).type == TOKEN_CONST) {
			consume_token(parser);
			isConst = true;
		}

		char *type = parse_type(parser);

		if (current_token(parser).type == TOKEN_MUL) {
			consume_token(parser);
			isPtr = true;
		}

		char *param_name = expect(parser, TOKEN_IDENTIFIER, "Expected identifier.").lexeme;

		AstNode *param_data = create_param_data(
			param_name,
			type,
			isConst,
			isPtr
		);

		AstNode *param_node = create_ast_node(
			NODE_PARAMETER,
			param_data,
			line_param,
			column_start_param,
			position_start_param,
			current_token(parser).column_end - 1,
			current_token(parser).position_end - 1
		);

		parameters_data->parameters = realloc(
			parameters_data->parameters,
			sizeof(AstNode *) * (parameters_data->parameter_count + 1)
		);

		if (!parameters_data->parameters) {
			fprintf(stderr, "Error: Failed to reallocate memory for parameters");
			exit(EXIT_FAILURE);
		}

		parameters_data->parameters[parameters_data->parameter_count] = param_node;
		parameters_data->parameter_count++;

		if (current_token(parser).type == TOKEN_COMMA) {
			consume_token(parser);
		} else if (current_token(parser).type != TOKEN_CPAREN) {
			expect(parser, TOKEN_COMMA, "Expected \",\" or \")\" after parameter.");
		}
	}

	expect(parser, TOKEN_CPAREN, "Expected \")\".");
	expect(parser, TOKEN_ARROW, "Expected \"->\".");

	char *type = parse_type(parser);

	bool isPtr = false;

	if (current_token(parser).type == TOKEN_MUL) {
		consume_token(parser);
		isPtr = true;
	}

	expect(parser, TOKEN_COLON, "Expected \":\".");

	FunctionNode *function_data = MALLOC_S(sizeof(FunctionNode));
	function_data->body = NULL;
	function_data->body_count = 0;
	function_data->name = name;
	function_data->type = type;
	function_data->isPtr = isPtr;
  function_data->parameters = parameters_data;

	while (not_eof(parser) && current_token(parser).type != TOKEN_END) {
		function_data->body = REALLOC_S(
				function_data->body,
				sizeof(AstNode *) * (function_data->body_count + 1)
		);
		function_data->body[function_data->body_count++] = parse_stmt(parser);
	}

	expect(parser, TOKEN_END, "Expected end.");

	int column_end = current_token(parser).column_end - 1;
	int position_end = current_token(parser).position_end - 1;

	expect(parser, TOKEN_SEMICOLON, "Expected \";\".");

	AstNode *function_node = create_ast_node(
		NODE_FUNCTION,
		function_data,
		line,
		column_start,
		position_start,
		column_end,
		position_end
	);

	return function_node;
}
