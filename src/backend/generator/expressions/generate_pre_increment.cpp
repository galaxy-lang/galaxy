#include "backend/generator/expressions/generate_pre_increment.hpp"
#include "backend/generator/expressions/generate_expr.hpp"

llvm::Value *generate_pre_increment(PreIncrementExpr *node, llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder) {
    llvm::Value *Operand = generate_expr(node->op, Context, Builder);
    llvm::Value *Incremented = Builder.CreateAdd(Operand, llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 1), "incpretmp");
    return Incremented;
}