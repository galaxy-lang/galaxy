#include "backend/generator/expressions/generate_logical_not.hpp"
#include "backend/generator/expressions/generate_expr.hpp"

llvm::Value *generate_logical_not(LogicalNotExpr *node, llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder) {
    llvm::Value *Operand = generate_expr(node->op, Context, Builder);
    return Builder.CreateICmpEQ(Operand, llvm::ConstantInt::getFalse(Context), "nottmp");
}