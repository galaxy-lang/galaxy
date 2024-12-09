#include "backend/generator/statements/generate_variable_declaration_stmt.hpp"
#include "backend/generator/expressions/generate_expr.hpp"

llvm::Value* generate_variable_declaration_stmt(VariableNode *node, llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder) {
    // Determinar o tipo da variável
    llvm::Type *var_type;
    switch (node->varType) {
        case TYPE_INT: var_type = llvm::Type::getInt32Ty(Context); break;
        case TYPE_FLOAT: var_type = llvm::Type::getFloatTy(Context); break;
        case TYPE_DOUBLE: var_type = llvm::Type::getDoubleTy(Context); break;
        case TYPE_BOOL: var_type = llvm::Type::getInt1Ty(Context); break;
        default: var_type = llvm::Type::getInt32Ty(Context); break;
    }

    // Criar espaço para a variável no IR
    llvm::AllocaInst *alloca = Builder.CreateAlloca(var_type, nullptr, node->name);

    // Inicializar a variável, se um valor inicial for fornecido
    if (node->value != nullptr) {
        llvm::Value *init_value = generate_expr(node->value, Context, Builder);
        if (init_value->getType() != var_type) {
            throw std::runtime_error("Tipo do valor inicial não corresponde ao tipo da variável");
        }
        Builder.CreateStore(init_value, alloca);
    }

    return alloca;
}
