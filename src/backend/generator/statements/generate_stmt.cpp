#include "backend/generator/statements/generate_stmt.hpp"
#include "backend/generator/statements/generate_variable_declaration_stmt.hpp"
#include "backend/generator/statements/generate_function_declaration_stmt.hpp"
#include "backend/generator/statements/generate_extern_stmt.hpp"
#include "backend/generator/statements/generate_for_stmt.hpp"
#include "backend/generator/statements/generate_if_stmt.hpp"
#include "backend/generator/expressions/generate_expr.hpp"

llvm::Value* generate_stmt(AstNode *node, llvm::LLVMContext &Context, llvm::Module &Module, llvm::IRBuilder<llvm::NoFolder> &Builder) {
    switch (node->kind) {
        case NODE_VARIABLE: {
            VariableNode *varNode = (VariableNode *)node->data;
            return generate_variable_declaration_stmt(varNode, Context, Builder, Module);
        }
        case NODE_FUNCTION: {
            FunctionNode *funcNode = (FunctionNode *)node->data;
            return generate_function_declaration_stmt(funcNode, Context, Builder, Module);
        }
        case NODE_FOR: {
            ForNode *forNode = (ForNode *)node->data;
            return generate_for_stmt(forNode, Context, Builder, Module);
        }
        case NODE_IF: {
            IfNode *ifNode = (IfNode *)node->data;
            return generate_if_stmt(ifNode, Context, Builder, Module);
        }
        case NODE_EXTERN: {
            ExternNode *externNode = (ExternNode *)node->data;
            return generate_extern_stmt(externNode, Context, Builder, Module);
        }
        default: {
            return generate_expr(node, Context, Builder, Module); // Para expressões, o valor gerado
        }
    }
}
