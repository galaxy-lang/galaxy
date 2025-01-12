#ifndef GENERATE_VARIABLE_DECLARATION_STMT_H
#define GENERATE_VARIABLE_DECLARATION_STMT_H

extern "C" {
  #include "frontend/ast/definitions.h"
}
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/NoFolder.h>

llvm::Value* generate_variable_declaration_stmt(VariableNode *node, llvm::LLVMContext &Context, llvm::IRBuilder<llvm::NoFolder> &Builder, llvm::Module &Module);

#endif // GENERATE_VARIABLE_DECLARATION_STMT_H