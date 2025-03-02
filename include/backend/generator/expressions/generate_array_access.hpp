#ifndef GENERATE_ARRAY_ACCESS_H
#define GENERATE_ARRAY_ACCESS_H

extern "C" {
  #include "frontend/ast/definitions.h"
}
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include "llvm/IR/Module.h"
#include <llvm/IR/NoFolder.h>

llvm::Value *generate_array_access(ArrayAccessNode *array_access_node, llvm::LLVMContext &Context, llvm::IRBuilder<llvm::NoFolder> &Builder, llvm::Module &Module);

#endif // GENERATE_ARRAY_ACCESS_H