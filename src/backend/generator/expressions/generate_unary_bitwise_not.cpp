#include "backend/generator/expressions/generate_unary_bitwise_not.hpp"
#include "backend/generator/expressions/generate_expr.hpp"

llvm::Value *generate_unary_bitwise_not(UnaryBitwiseNotExpr *node, llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder) {
    llvm::Value *Operand = generate_expr(node->op, Context, Builder);
    return Builder.CreateNot(Operand, "notbitwisetmp");
}