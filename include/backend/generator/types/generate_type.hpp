#ifndef GENERATE_TYPE_H
#define GENERATE_TYPE_H

extern "C" {
  #include "frontend/parser/core.h"
}
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/BasicBlock.h>

llvm::Type* generate_type(const char* parsed_type, llvm::LLVMContext &Context);

#endif // GENERATE_TYPE_H
