#include "../../../../include/parser/printer/nodes/print_package.h"
#include "../../../../../include/ast/definitions.h"

void print_package(const AstNode *node, int depth) {
  PackageNode *package_data = (PackageNode *)node->data;

  if (package_data) {
    print_indent(depth + 1);
    printf("Package Name: %s\n", package_data->package);
  }
}