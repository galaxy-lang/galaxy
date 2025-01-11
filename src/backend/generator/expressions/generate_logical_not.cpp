#include "backend/generator/expressions/generate_logical_not.hpp"
#include "backend/generator/expressions/generate_expr.hpp"

llvm::Value *generate_logical_not(LogicalNotExpr *node, llvm::LLVMContext &Context, llvm::IRBuilder<llvm::NoFolder> &Builder, llvm::Module &Module) {
    // Generate LLVM IR for the operand (the expression being negated)
    llvm::Value *Operand = generate_expr(node->op, Context, Builder, Module);

    // Create an integer comparison instruction (ICmpEQ) to check if the operand is equal to false (0).
    // If the operand is false (0), the result will be true (1), and vice versa.
    return Builder.CreateICmpEQ(Operand, llvm::ConstantInt::getFalse(Context), "nottmp");
}