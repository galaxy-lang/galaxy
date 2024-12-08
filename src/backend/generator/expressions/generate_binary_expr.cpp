#include "backend/generator/expressions/generate_binary_expr.hpp"
#include "backend/generator/expressions/generate_expr.hpp"

llvm::Value *generate_binary_expr(BinaryExprNode *node, llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder) {
    llvm::Value *L = generate_expr(node->left, Context, Builder);
    llvm::Value *R = generate_expr(node->right, Context, Builder);

    if (node->op == "+") return Builder.CreateFAdd(L, R, "addtmp");
    if (node->op == "-") return Builder.CreateFSub(L, R, "subtmp");
    if (node->op == "*") return Builder.CreateFMul(L, R, "multmp");
    if (node->op == "/") return Builder.CreateFDiv(L, R, "divtmp");

    throw std::runtime_error("Unknown binary operator");
}