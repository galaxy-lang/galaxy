#include "backend/generator/expressions/generate_pre_decrement.hpp"
#include "backend/generator/expressions/generate_expr.hpp"

llvm::Value *generate_pre_decrement(PreDecrementExpr *node, llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder){
    llvm::Value *Operand = generate_expr(node->op, Context, Builder);
    llvm::Value *Decremented = Builder.CreateSub(Operand, llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 1), "decpretmp");
    return Decremented;
}