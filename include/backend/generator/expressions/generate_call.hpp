#ifndef GENERATE_CALL_H
#define GENERATE_CALL_H

extern "C" {
  #include "frontend/ast/definitions.h"
}
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>

llvm::Value *generate_call(CallNode *call_node, llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder, llvm::Module &Module);

#endif // GENERATE_CALL_H