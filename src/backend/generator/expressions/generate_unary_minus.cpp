#include "backend/generator/expressions/generate_unary_minus.hpp"
#include "backend/generator/expressions/generate_expr.hpp"

llvm::Value *generate_unary_minus(UnaryMinusExpr *node, llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder) {
    llvm::Value *Operand = generate_expr(node->op, Context, Builder);
    return Builder.CreateFNeg(Operand, "negtmp");
}