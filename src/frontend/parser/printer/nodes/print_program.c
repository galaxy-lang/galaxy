#include "../../../../../include/frontend/parser/printer/nodes/print_program.h"
#include "../../../../../include/frontend/parser/printer/visited.h"
#include "../../../../../include/frontend/ast/definitions.h"
#include "../../../../../include/frontend/parser/printer/print_indent.h"
#include "../../../../../include/frontend/parser/printer/print_ast.h"

void print_program(const AstNode *node, int depth, VisitedNodes *visited)
{
  print_indent(depth + 1);
  ProgramNode *program_data = (ProgramNode *)node->data;

  if (program_data && program_data->statements)
  {
    printf("Program has %zu statements\n", program_data->statement_count);
    for (size_t i = 0; i < program_data->statement_count; i++)
    {
      AstNode *node = program_data->statements[i];
      print_ast_node(node, depth + 1, visited);
    }
  }
  else
  {
    printf("Value: (No statements or NULL data)\n");
  }
}