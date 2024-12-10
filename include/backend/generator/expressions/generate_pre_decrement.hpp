#ifndef GENERATE_PRE_DECREMENT_H
#define GENERATE_PRE_DECREMENT_H

extern "C" {
  #include "frontend/ast/definitions.h"
}
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>

llvm::Value *generate_pre_decrement(PreDecrementExpr *node, llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder, llvm::Module &Module);

#endif // GENERATE_PRE_DECREMENT_H