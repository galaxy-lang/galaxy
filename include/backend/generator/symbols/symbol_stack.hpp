#ifndef SYMBOL_STACK_H
#define SYMBOL_STACK_H

#include <string>
#include <unordered_map>
#include <stack>
#include <llvm/IR/Value.h>
#include <mutex>  // Include for std::mutex

struct SymbolInfo {
    llvm::Value* declaration;
    llvm::Value* value;
    llvm::Type* type;
};

using SymbolTable = std::unordered_map<std::string, SymbolInfo>;

extern std::stack<SymbolTable> symbol_stack;
extern std::mutex symbol_stack_mutex;

void enter_scope(void);
void exit_scope(void);

#endif // SYMBOL_STACK_H
