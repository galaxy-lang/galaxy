#ifndef GENERATE_BINARY_EXPR_H
#define GENERATE_BINARY_EXPR_H

extern "C" {
  #include "frontend/ast/definitions.h"
}
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"

llvm::Value *generate_binary_expr(BinaryExprNode *node, llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder, llvm::Module &Module);

#endif // GENERATE_BINARY_EXPR_H