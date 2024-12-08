#ifndef GENERATE_LOGICAL_NOT
#define GENERATE_LOGICAL_NOT

extern "C" {
  #include "frontend/ast/definitions.h"
}
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>

llvm::Value *generate_logical_not(LogicalNotExpr *node, llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder);

#endif // GENERATE_LOGICAL_NOT