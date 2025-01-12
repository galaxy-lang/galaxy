#ifndef GENERATE_PRE_DECREMENT_H
#define GENERATE_PRE_DECREMENT_H

extern "C" {
  #include "frontend/ast/definitions.h"
}
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/NoFolder.h>

llvm::Value *generate_pre_decrement(PreDecrementExpr *node, llvm::LLVMContext &Context, llvm::IRBuilder<llvm::NoFolder> &Builder, llvm::Module &Module);

#endif // GENERATE_PRE_DECREMENT_H