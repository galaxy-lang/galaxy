#include "backend/generator/expressions/generate_pre_decrement.hpp"
#include "backend/generator/expressions/generate_expr.hpp"

llvm::Value *generate_pre_decrement(PreDecrementExpr *node, llvm::LLVMContext &Context, llvm::IRBuilder<llvm::NoFolder> &Builder, llvm::Module &Module) {
    // Generate LLVM IR for the operand (the value to be decremented)
    llvm::Value *Operand = generate_expr(node->op, Context, Builder, Module);
    
    Operand = Builder.CreatePtrToInt(Operand, llvm::Type::getInt32Ty(Context));

    // Create a subtraction instruction to decrement the operand by 1
    // This is a pre-decrement operation, so the operand is decreased before returning the result.
    llvm::Value *Decremented = Builder.CreateSub(Operand, llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 1), "decpretmp");
    
    // Return the decremented value
    return Decremented;
}