#include "backend/generator/expressions/generate_numeric_literal.hpp"

llvm::Value *generate_numeric_literal(NumericLiteralNode *node, llvm::LLVMContext &Context) {
    if (node->decimal) {
        return llvm::ConstantFP::get(Context, llvm::APFloat(node->value));
    } else {
        return llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), (int)node->value);
    }
}