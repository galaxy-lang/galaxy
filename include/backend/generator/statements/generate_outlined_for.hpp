#ifndef GENERATE_OUTLINED_FOR_H
#define GENERATE_OUTLINED_FOR_H

extern "C" {
  #include "frontend/ast/definitions.h"
}

#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>

llvm::Function* generate_outlined_for(ForNode *node, llvm::LLVMContext &Context, llvm::Module &Module, llvm::GlobalVariable *ompIdent, char *schedule_policy);

#endif // GENERATE_OUTLINED_FOR_H