#ifndef ITERATOR_STACK_H
#define ITERATOR_STACK_H

#include <stack>
#include <string>
#include <llvm/IR/Value.h>
#include <mutex>

struct IteratorInfo {
    llvm::Value* declaration;
    llvm::Value* value;
    llvm::Type* type;
};

extern std::stack<IteratorInfo> iterator_stack;
extern std::mutex iterator_stack_mutex;

void push_iterator(llvm::Value* declaration, llvm::Value* value, llvm::Type* type);
IteratorInfo pop_iterator();
IteratorInfo* current_iterator();

#endif // ITERATOR_STACK_H
