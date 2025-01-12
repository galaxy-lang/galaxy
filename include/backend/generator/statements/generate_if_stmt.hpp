#ifndef GENERATE_IF_STMT_H
#define GENERATE_IF_STMT_H

extern "C" {
  #include "frontend/ast/definitions.h"
}
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/NoFolder.h>

llvm::Value* generate_if_stmt(IfNode *node, llvm::LLVMContext &Context, llvm::IRBuilder<llvm::NoFolder> &Builder, llvm::Module &Module);

#endif // GENERATE_IF_STMT_H