#include "../../../include/args/definitions.h";

void free_arg_parse(ArgParseResult *result) {
  free(result->named_args);
  free(result->positional_args);
  free(result);
}

ArgParseResult arg_parse(int argc, char *argv[]) {
  ArgParseResult result;

  result.named_args = calloc(argc, sizeof(Argument));
  result.named_count = 0;
  result.positional_args = calloc(argc, sizeof(char *));
  result.positional_count = 0;

  for (int i = 0; i < argc; i++ && argv[i + 1][0] != '-')
  {
    if (argv[i][0] == '-') {
      result.named_args[result.named_count].flag = argv[i];
      result.named_args[result.named_count].value = argv[i + 1];
      result.named_count++;
      i++;
    } else {
      result.positional_args[result.positional_count] = argv[i];
      result.positional_count++;
    }
  }

  return result;
}