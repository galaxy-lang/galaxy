#ifndef GENERATE_IR_H
#define GENERATE_IR_H

#include <vector>
extern "C" {
  #include "frontend/ast/definitions.h"
}
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/NoFolder.h>

std::vector<llvm::Value*> generate_ir(AstNode *node, llvm::LLVMContext &Context, llvm::Module &Module, llvm::IRBuilder<llvm::NoFolder> &Builder);

#endif // GENERATE_IR_H