#ifndef GENERATE_STRING_LITERAL_H
#define GENERATE_STRING_LITERAL_H

extern "C" {
  #include "frontend/ast/definitions.h"
}
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include "llvm/IR/Module.h"
#include <llvm/IR/NoFolder.h>

llvm::Value *generate_string_literal(StringNode *string_node, llvm::LLVMContext &Context, llvm::IRBuilder<llvm::NoFolder> &Builder, llvm::Module &Module);

#endif // GENERATE_STRING_LITERAL_H