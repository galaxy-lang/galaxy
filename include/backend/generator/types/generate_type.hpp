#ifndef GENERATE_FUNCTION_DECLARATION_STMT_H
#define GENERATE_FUNCTION_DECLARATION_STMT_H

extern "C" {
  #include "frontend/parser/core.h"
}
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/BasicBlock.h>

llvm::Type* generate_type(Parser *parser, llvm::LLVMContext &Context, const char* parsed_type);

#endif // GENERATE_TYPE_H
