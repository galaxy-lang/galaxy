#ifndef GENERATE_ASSIGNMENT_EXPR_H
#define GENERATE_ASSIGNMENT_EXPR_H

extern "C" {
  #include "frontend/ast/definitions.h"
}
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>

llvm::Value *generate_assignment_expr(AssignmentNode *node, llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder);

#endif // GENERATE_ASSIGNMENT_EXPR_H