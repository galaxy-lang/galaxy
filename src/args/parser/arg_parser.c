#include "../../../include/args/definitions.h";

/**
 * @brief Frees the memory allocated for named and positional arguments in ArgParseResult.
 *
 * This function releases the dynamically allocated memory for the named arguments
 * and positional arguments contained within the ArgParseResult structure.
 *
 * @param result Pointer to the ArgParseResult structure whose memory is to be freed.
 */
void free_arg_parse(ArgParseResult *result) {
  free(result->named_args);
  free(result->positional_args);
}

/**
 * @brief Parse the command line arguments into named and positional arguments.
 *
 * @param argc The number of command line arguments.
 * @param argv The array of command line arguments.
 * @return The parsed command line arguments.
 */
ArgParseResult arg_parse(int argc, char *argv[]) {
  ArgParseResult result;

  result.named_args = calloc(argc, sizeof(Argument));
  result.named_count = 0;
  result.positional_args = calloc(argc, sizeof(char *));
  result.positional_count = 0;

  for (int i = 0; i < argc; i++) {
    if (i + 1 < argc && argv[i][0] == '-') {
      result.named_args[result.named_count].flag = argv[i];
      result.named_args[result.named_count].value = argv[i + 1];
      result.named_count++;
      i++;
    }
    else {
      result.positional_args[result.positional_count] = argv[i];
      result.positional_count++;
    }
  }

  return result;
}
