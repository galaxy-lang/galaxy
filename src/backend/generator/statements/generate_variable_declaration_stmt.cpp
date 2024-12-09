#include "backend/generator/statements/generate_variable_declaration_stmt.hpp"
#include "backend/generator/expressions/generate_expr.hpp"
#include "backend/generator/types/generate_type.hpp"

llvm::Value* generate_variable_declaration_stmt(VariableNode *node, llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder) {
    llvm::Type *var_type = generate_type(nullptr, Context, node->varType);

    llvm::AllocaInst *alloca = Builder.CreateAlloca(var_type, nullptr, node->name);

    if (node->value != nullptr) {
        llvm::Value *init_value = generate_expr(node->value, Context, Builder);
        if (init_value->getType() != var_type) {
            throw std::runtime_error("Tipo do valor inicial não corresponde ao tipo da variável");
        }
        Builder.CreateStore(init_value, alloca);
    }

    return alloca;
}
