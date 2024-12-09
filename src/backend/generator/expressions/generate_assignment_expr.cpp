#include "backend/generator/expressions/generate_assignment_expr.hpp"
#include "backend/generator/expressions/generate_expr.hpp"

llvm::Value *generate_assignment_expr(AssignmentNode *node, llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder, llvm::Module &Module) {
    // Evaluate the right-hand side expression and get its LLVM value representation.
    llvm::Value *right_value = generate_expr(node->value, Context, Builder, Module);

    // Evaluate the left-hand side expression to get the memory location (pointer).
    llvm::Value *left_value = generate_expr(node->left, Context, Builder, Module);

    // Create a store instruction that assigns the value of the
    // right-hand side to the memory location of the left-hand side.
    Builder.CreateStore(right_value, left_value);

    // Return a null value of the type of the left operand, 
    // as assignment expressions generally do not return a value.
    return llvm::Constant::getNullValue(left_value->getType());
}

