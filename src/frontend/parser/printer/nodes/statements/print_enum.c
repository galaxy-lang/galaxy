#include "frontend/parser/printer/nodes/statements/print_enum.h"

void print_enum(const AstNode *node, int depth, VisitedNodes *visited)
{
  EnumNode *enum_data = (EnumNode *)node->data;

  print_indent(depth + 1);
  printf("Name: %s\n", enum_data->name);

  if (enum_data && enum_data->item_count > 0)
  {
    for (int i = 0; i < enum_data->item_count; i++)
    {
      AstNode *item = enum_data->data[i];
      if (item && item->kind)
      {
        if (item->kind == NODE_NUMERIC_LITERAL)
        {
          print_ast_node(item, depth + 4, visited);
          continue;
        }

        print_ast_node(item, depth + 2, visited);
      }
      else
      {
        fprintf(stderr, "Error: Invalid enum item\n");
      }
    }
  }
}