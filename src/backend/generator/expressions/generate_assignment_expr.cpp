#include "backend/generator/expressions/generate_assignment_expr.hpp"
#include "backend/generator/expressions/generate_expr.hpp"
#include <stdexcept>

llvm::Value *generate_assignment_expr(AssignmentNode *node, llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder) {
    llvm::Value *right_value = generate_expr(node->value, Context, Builder);

    llvm::Value *left_value = generate_expr(node->left, Context, Builder);

    if (!left_value->getType()->isPointerTy()) {
        throw std::runtime_error("O lado esquerdo deve ser uma variável ou ponteiro");
    }

    Builder.CreateStore(right_value, left_value);

    return llvm::Constant::getNullValue(left_value->getType());
}
