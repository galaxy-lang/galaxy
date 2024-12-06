#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "frontend/ast/core.h"
#include "frontend/ast/definitions.h"
#include "frontend/lexer/core.h"
#include "utils.h"
#include "frontend/types.h"

/**
 * @brief Creates a new AST node with the given parameters.
 *
 * This function allocates memory for a new AST node and initializes its fields
 * with the provided parameters. The node's children are initially set to NULL and
 * the child count is set to 0.
 *
 * @param kind The type of node (e.g., NODE_PROGRAM, NODE_IDENTIFIER, etc.).
 * @param data A pointer to the specific data associated with the node.
 * @param line The line number of the node in the source code.
 * @param column_start The starting column number of the node in the source code.
 * @param position_start The starting position of the node in the source code.
 * @param column_end The ending column number of the node in the source code.
 * @param position_end The ending position of the node in the source code.
 *
 * @return A pointer to the newly created AST node.
 */
AstNode *create_ast_node(
    NodeType kind,
    void *data,
    int line,
    int column_start,
    int position_start,
    int column_end,
    int position_end)
 {
    AstNode *node = MALLOC_S(sizeof(AstNode));
    node->kind = kind;
    node->data = data;
    node->line = line;
    node->column_start = column_start;
    node->position_start = position_start;
    node->column_end = column_end;
    node->position_end = position_end;
    node->child_count = 0;
    node->children = NULL;
    return node;
}

/**
 * @brief Creates the parent node to manage the child nodes.
 *
 * This function manipulate the hierarchical data structure
 * represented by the AST to allow the children node to be dinamically
 * added to the parent node.
 *
 * @param parent: parent node.
 * @param child: child node.
 * @return void.
 */

void add_child_to_node(AstNode *parent, AstNode *child)
{
  if (!parent || !child)
  {
    fprintf(stderr, "Error: Trying to add NULL node as child\n");
    return;
  }

  AstNode **temp = realloc(parent->children, (parent->child_count + 1) * sizeof(AstNode *));
  if (!temp)
  {
    fprintf(stderr, "Error: Memory allocation failed\n");
    return;
  }
  parent->children = temp;
  parent->children[parent->child_count++] = child;
}

/**
 * @brief Creates the numeric literal to be stored in data pointer.
 *
 * This function creates a numeric literal to be stored in the data pointer
 * return by the value variable.
 *
 * @param value: Store the fixed numeric literal stored by the pointer.
 * @return data: Returns the data with numeric literal number allocated.
 */

void *create_binary_expr_data(AstNode *left, AstNode *right, char *operator)
{
  BinaryExprNode *data = MALLOC_S(sizeof(BinaryExprNode));
  data->left = left;
  data->right = right;
  data->operator = operator;
  return data;
}

void *create_numeric_literal_data(double value) {
    NumericLiteralNode *data = MALLOC_S(sizeof(NumericLiteralNode));
    data->value = value;
    return data;
}

void *create_identifier_data(const char *symbol) {
    IdentifierNode *data = MALLOC_S(sizeof(IdentifierNode));
    data->symbol = strdup(symbol);
    return data;
}

void *create_package_data(char *package) {
    PackageNode *data = MALLOC_S(sizeof(PackageNode));
    data->package = package;
    return data;

}

void *create_property_data(char *key, AstNode *value) {
    PropertyNode *data = MALLOC_S(sizeof(PropertyNode));
    data->key = key;
    data->value = value;
    return data;
}

void *create_variable_data(char *name, AstNode *value, bool isPtr, bool isConst, Type varType) {
    VariableNode *data = MALLOC_S(sizeof(VariableNode));
    data->name = name;
    data->value = value;
    data->isPtr = isPtr;
    data->isConst = isConst;
    data->varType = varType;
    return data;
}

void *create_param_data(char *name, Type type, bool isConst, bool isPtr) {
  ParameterNode *data = MALLOC_S(sizeof(ParameterNode));
  data->name = name;
  data->type = type;
  data->isConst = isConst;
  data->isPtr = isPtr;
  return data;
}


/**
 * @brief Recursively frees memory allocated for an AST node and its children.
 *
 * This function deallocates memory for a given AST node, its data, and all its
 * child nodes. It handles different node types (program, numeric literal,
 * identifier, and binary expression) appropriately.
 *
 * @param node Pointer to the AstNode to be freed. If NULL, the function returns
 *             without doing anything.
 *
 * @return void This function does not return a value.
 */
void free_ast_node(AstNode *node)
{
    if (!node)
        return;

    // Free children first
    for (size_t i = 0; i < node->child_count; i++)
    {
        free_ast_node(node->children[i]);
    }
    free(node->children);

    // Then handle the node's specific data
    switch (node->kind)
    {
    case NODE_PROGRAM:
    {
        ProgramNode *data = (ProgramNode *)node->data;
        if (data)
        {
            for (size_t i = 0; i < data->statement_count; i++)
            {
                free_ast_node(data->statements[i]);
            }
            free(data->statements);
        }
        break;
    }
    case NODE_IDENTIFIER:
    {
        IdentifierNode *data = (IdentifierNode *)node->data;
        if (data && data->symbol)
        {
            free(data->symbol);
        }
        break;
    }
    case NODE_BINARY_EXPR:
    {
        BinaryExprNode *data = (BinaryExprNode *)node->data;
        if (data && data->operator)
        {
            free(data->operator);
        }
        break;
    }
    case NODE_VARIABLE:
    {
        VariableNode *data = (VariableNode *)node->data;
        if (data)
        {
            if (data->name)
                free(data->name);
            // Type freeing if dynamically allocated
        }
        break;
    }
    case NODE_FUNCTION:
    {
        FunctionNode *data = (FunctionNode *)node->data;
        if (data)
        {
            if (data->name)
                free(data->name);
            if (data->parameters)
            {
                for (int i = 0; i < data->parameters->parameter_count; i++)
                {
                    free(data->parameters->parameters[i]);
                }
                free(data->parameters->parameters);
                free(data->parameters);
            }
            if (data->body)
            {
                for (size_t i = 0; i < data->body_count; i++)
                {
                    free_ast_node(data->body[i]);
                }
                free(data->body);
            }
        }
        break;
    }
    case NODE_IMPORT:
    {
        ImportNode *data = (ImportNode *)node->data;
        if (data)
        {
            for (size_t i = 0; i < data->package_count; i++)
            {
                free_ast_node(data->packages[i]);
            }
            free(data->packages);
        }
        break;
    }
    case NODE_OBJECT:
    {
        ObjectNode *data = (ObjectNode *)node->data;
        if (data)
        {
            for (size_t i = 0; i < data->property_count; i++)
            {
                free_ast_node(data->properties[i]);
            }
            free(data->properties);
        }
        break;
    }
    case NODE_PROPERTY:
    {
        PropertyNode *data = (PropertyNode *)node->data;
        if (data)
        {
            if (data->key)
                free(data->key);
            free_ast_node(data->value);
        }
        break;
    }
    case NODE_NUMERIC_LITERAL:
    {
        // No dynamically allocated fields
        break;
    }

    // Finally, free the node's data and the node itself
    if (node->data)
    {
        free(node->data);
    }
    free(node);
  }
}