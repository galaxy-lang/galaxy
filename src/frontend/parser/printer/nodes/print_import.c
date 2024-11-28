#include "../../../../../include/parser/printer/nodes/print_import.h"
#include "../../../../../include/ast/definitions.h"
#include "../../../../../include/parser/printer/print_indent.h"

void print_import(const AstNode *node, int depth) {
  ImportNode *import_data = (ImportNode *)node->data;
  
  if (import_data && import_data->package_count > 0) {
    for (int i = 0; i < import_data->package_count; i++) {
      print_indent(depth + 1);
      printf("Package: %s\n", import_data->packages[i]);
    }
  }
}