#ifndef GENERATE_UNARY_MINUS_H
#define GENERATE_UNARY_MINUS_H

extern "C" {
  #include "frontend/ast/definitions.h"
}
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/NoFolder.h>

llvm::Value *generate_unary_minus(UnaryMinusExpr *node, llvm::LLVMContext &Context, llvm::IRBuilder<llvm::NoFolder> &Builder, llvm::Module &Module);

#endif // GENERATE_UNARY_MINUS_H