#ifndef ARG_PARSE_DEF
#define ARG_PARSE_DEF

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  TARGET
} ArgTypes;

typedef struct {
  char* flag;
  char *value;
} Argument;

typedef struct {
    Argument *named_args;
    int named_count;
    char **positional_args;
    int positional_count;
} ArgParseResult;

ArgParseResult arg_parse(int argc, char *argv[]);
void free_arg_parse(ArgParseResult *result);

#endif