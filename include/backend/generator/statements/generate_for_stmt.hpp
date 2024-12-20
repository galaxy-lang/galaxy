#ifndef GENERATE_FOR_STMT_H
#define GENERATE_FOR_STMT_H

extern "C" {
  #include "frontend/ast/definitions.h"
}
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/BasicBlock.h>

llvm::Value* generate_for_stmt(ForNode *node, llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder, llvm::Module &Module);

#endif // GENERATE_FOR_STMT_H