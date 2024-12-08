#include "backend/generator/statements/generate_stmt.hpp"
#include "backend/generator/statements/generate_variable_declaration_stmt.hpp"
#include "backend/generator/statements/generate_function_declaration_stmt.hpp"
#include "backend/generator/expressions/generate_expr.hpp"

llvm::Value* generate_stmt(AstNode *node, llvm::LLVMContext &Context, llvm::Module &Module, llvm::IRBuilder<> &Builder) {
    switch (node->kind) {
        case NODE_VARIABLE: {
            VariableNode *varNode = (VariableNode *)node->data;
            generate_variable_declaration_stmt(varNode, Context, Builder);
            return nullptr;
        }
        case NODE_FUNCTION: {
            FunctionNode *funcNode = (FunctionNode *)node->data;
            generate_function_declaration_stmt(funcNode, Context, Module, Builder);
            return nullptr;
        }
        default: {
            return generate_expr(node, Context, Builder);
        }
    }
}
