#include "backend/generator/expressions/generate_numeric_literal.hpp"

llvm::Value *generate_numeric_literal(NumericLiteralNode *node, llvm::LLVMContext &Context) {
    if (node->decimal) {
        // Create a floating-point constant using APFloat, which represents arbitrary precision floats
        return llvm::ConstantFP::get(Context, llvm::APFloat(node->value));
    } else {
        // For non-decimal values, create a 32-bit integer constant.
        // Type::getInt32Ty(Context) provides the correct integer type for the context
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), (int)node->value);
    }
}
