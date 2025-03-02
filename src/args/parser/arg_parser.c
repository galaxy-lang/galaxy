#include "args/definitions.h"

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

  // Aloca memória para os argumentos nomeados e posicionais
  result.named_args = (Argument *)calloc(argc, sizeof(Argument));
  result.named_count = 0;
  result.positional_args = (char **)calloc(argc, sizeof(char *));
  result.positional_count = 0;

  for (int i = 1; i < argc; i++) { // Começa em 1 para ignorar o nome do programa
      if (argv[i][0] == '-') { // Verifica se é um argumento nomeado
          if (argv[i][1] == '-') { // Formato --flag=value
              char *flag = argv[i];
              char *equal_sign = strchr(flag, '='); // Procura o '=' no argumento
              if (equal_sign) {
                  // Divide o argumento em flag e valor
                  *equal_sign = '\0'; // Separa a flag do valor
                  result.named_args[result.named_count].flag = flag;
                  result.named_args[result.named_count].value = equal_sign + 1;
                  result.named_count++;
              } else {
                  // Argumento nomeado sem valor (ex: --help)
                  result.named_args[result.named_count].flag = flag;
                  result.named_args[result.named_count].value = NULL;
                  result.named_count++;
              }
          } else { // Formato -flag value
              if (i + 1 < argc && argv[i + 1][0] != '-') { // Verifica se o próximo argumento não é uma flag
                  result.named_args[result.named_count].flag = argv[i];
                  result.named_args[result.named_count].value = argv[i + 1];
                  result.named_count++;
                  i++; // Avança para o próximo argumento
              } else {
                  // Argumento nomeado sem valor (ex: -o)
                  result.named_args[result.named_count].flag = argv[i];
                  result.named_args[result.named_count].value = NULL;
                  result.named_count++;
              }
          }
      } else { // Argumento posicional
          result.positional_args[result.positional_count] = argv[i];
          result.positional_count++;
      }
  }

  return result;
}
