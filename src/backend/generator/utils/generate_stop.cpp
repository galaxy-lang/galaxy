#include "backend/generator/expressions/generate_expr.hpp"
#include "backend/generator/utils/generate_stop.hpp"
#include "backend/generator/utils/return_id.hpp"

llvm::Value *generate_stop(AstNode *node, llvm::LLVMContext &Context, llvm::IRBuilder<llvm::NoFolder> &Builder, llvm::Module &Module) {
  BinaryExprNode *bin_expr = (BinaryExprNode *)node->data;

  global_id_return = "declaration";
  llvm::Value *expr = generate_expr(bin_expr->right, Context, Builder, Module);
  global_id_return = "value";

  return expr;
}