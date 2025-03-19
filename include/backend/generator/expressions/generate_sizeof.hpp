#ifndef GENERATE_SIZEOF_H
#define GENERATE_SIZEOF_H

extern "C" {
  #include "frontend/ast/definitions.h"
}
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include "llvm/IR/Module.h"
#include <llvm/IR/NoFolder.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>

llvm::Value *generate_sizeof(SizeofNode *sizeofNode, llvm::LLVMContext &Context, llvm::IRBuilder<llvm::NoFolder> &Builder, llvm::Module &Module);

#endif // GENERATE_SIZEOF_H