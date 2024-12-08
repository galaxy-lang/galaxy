#ifndef GENERATE_STMT_H
#define GENERATE_STMT_H

extern "C" {
  #include "frontend/ast/definitions.h"
}
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>

llvm::Value* generate_stmt(AstNode *node, llvm::LLVMContext &Context, llvm::Module &Module, llvm::IRBuilder<> &Builder);

#endif // GENERATE_STMT_H