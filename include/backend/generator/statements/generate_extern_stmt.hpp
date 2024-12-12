#ifndef GENERATE_EXTERN_STMT
#define GENERATE_EXTERN_STMT

#include <map>
extern "C" {
  #include "frontend/ast/definitions.h"
}
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

llvm::Value* generate_extern_stmt(ExternNode *node, llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder, llvm::Module &Module);

#endif // GENERATE_EXTERN_STMT