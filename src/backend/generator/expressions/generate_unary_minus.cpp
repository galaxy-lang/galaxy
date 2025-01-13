#include "backend/generator/expressions/generate_unary_minus.hpp"
#include "backend/generator/expressions/generate_expr.hpp"

llvm::Value *generate_unary_minus(UnaryMinusExpr *node, llvm::LLVMContext &Context, llvm::IRBuilder<llvm::NoFolder> &Builder, llvm::Module &Module) {
    // Generate LLVM IR for the operand (the value to apply the unary minus operation on)
    llvm::Value *Operand = generate_expr(node->op, Context, Builder, Module);
    
    if (Operand->getType()->isDoubleTy()) {
        return Builder.CreateFNeg(Operand, "negtmp");
    } else {
        return Builder.CreateNeg(Operand, "negtmp");
    }
}