#ifndef GENERATE_EXPR_H
#define GENERATE_EXPR_H

extern "C" {
  #include "frontend/ast/definitions.h"
}
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>

llvm::Value *generate_expr(AstNode *node, llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder);

#endif // GENERATE_EXPR_H