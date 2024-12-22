#include "backend/generator/expressions/generate_expr.hpp"
#include "backend/generator/utils/generate_stop.hpp"

llvm::Value *generate_stop(AstNode *node, llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder, llvm::Module &Module) {
  BinaryExprNode *bin_expr = (BinaryExprNode *)node->data;
  return generate_expr(bin_expr->right, Context, Builder, Module);
}