#include "../../../../../include/parser/printer/nodes/print_object.h"
#include "../../../../../include/parser/printer/visited.h"
#include "../../../../../include/ast/definitions.h"
#include "../../../../../include/parser/printer/print_indent.h"
#include "../../../../../include/parser/printer/print_ast.h"

void print_object(const AstNode *node, int depth, VisitedNodes *visited){
  ObjectNode *object_data = (ObjectNode *)node->data;
            
  if (object_data && object_data->property_count > 0) {
    for (int i = 0; i < object_data->property_count; i++) {
      PropertyNode *property = (PropertyNode *)object_data->properties[i]->data;

      if (property) {
        print_indent(depth + 1);
        printf("Property: %s\n", property->key);

        if (property->value){
          print_ast_node(property->value, depth + 2, visited);
        }
      } 
    }
  }
}