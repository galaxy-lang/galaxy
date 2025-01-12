#include "backend/generator/expressions/generate_unary_bitwise_not.hpp"
#include "backend/generator/expressions/generate_expr.hpp"

llvm::Value *generate_unary_bitwise_not(UnaryBitwiseNotExpr *node, llvm::LLVMContext &Context, llvm::IRBuilder<llvm::NoFolder> &Builder, llvm::Module &Module) {
    // Generate LLVM IR for the operand (the value to apply the bitwise NOT operation on)
    llvm::Value *Operand = generate_expr(node->op, Context, Builder, Module);
    
    // Create a bitwise NOT instruction, which flips the bits of the operand
    llvm::Value *BitwiseNot = Builder.CreateNot(Operand, "notbitwisetmp");
    
    // Return the result of the bitwise NOT operation
    return BitwiseNot;
}
