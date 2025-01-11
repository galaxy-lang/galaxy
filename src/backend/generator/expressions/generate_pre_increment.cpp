#include "backend/generator/expressions/generate_pre_increment.hpp"
#include "backend/generator/expressions/generate_expr.hpp"

llvm::Value *generate_pre_increment(PreIncrementExpr *node, llvm::LLVMContext &Context, llvm::IRBuilder<llvm::NoFolder> &Builder, llvm::Module &Module) {
    // Generate LLVM IR for the operand (the value to be incremented)
    llvm::Value *Operand = generate_expr(node->op, Context, Builder, Module);

    Operand = Builder.CreatePtrToInt(Operand, llvm::Type::getInt32Ty(Context));
    
    // Create an addition instruction to increment the operand by 1
    // This is a pre-increment operation, so the operand is increased before returning the result.
    llvm::Value *Incremented = Builder.CreateAdd(Operand, llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 1), "incpretmp");
    
    // Return the incremented value
    return Incremented;
}
