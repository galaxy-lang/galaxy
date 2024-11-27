#ifndef PRINT_AST_NODE_H
#define PRINT_AST_NODE_H

void print_ast_node(const AstNode *node, int depth, VisitedNodes *visited);
const char* returnASTNodeName(NodeType node_type);
void print_ast(const AstNode *root);

#endif // PRINT_AST_NODE_H