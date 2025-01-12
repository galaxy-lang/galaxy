#include "backend/generator/expressions/generate_unary_minus.hpp"
#include "backend/generator/expressions/generate_expr.hpp"

llvm::Value *generate_unary_minus(UnaryMinusExpr *node, llvm::LLVMContext &Context, llvm::IRBuilder<llvm::NoFolder> &Builder, llvm::Module &Module) {
    // Generate LLVM IR for the operand (the value to apply the unary minus operation on)
    llvm::Value *Operand = generate_expr(node->op, Context, Builder, Module);
    
    // Create a floating-point negation instruction, which negates the operand (i.e., multiplies it by -1)
    llvm::Value *Negated = Builder.CreateNeg(Operand, "negtmp");
    
    // Return the result of the negation
    return Negated;
}