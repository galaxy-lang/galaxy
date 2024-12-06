#include "frontend/parser/printer/nodes/statements/print_package.h"
#include "frontend/ast/definitions.h"
#include "frontend/parser/printer/print_indent.h"

void print_package(const AstNode *node, int depth) {
  PackageNode *package_data = (PackageNode *)node->data;

  if (package_data) {
    print_indent(depth + 1);
    printf("Package Name: %s\n", package_data->package);
  }
}