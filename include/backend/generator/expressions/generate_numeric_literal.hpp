#ifndef GENERATE_NUMERIC_LITERAL_H
#define GENERATE_NUMERIC_LITERAL_H

extern "C" {
  #include "frontend/ast/definitions.h"
}
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>
#include <llvm/ADT/APFloat.h>

llvm::Value *generate_numeric_literal(NumericLiteralNode *node, llvm::LLVMContext &Context);

#endif // GENERATE_NUMERIC_LITERAL_H