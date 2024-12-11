#ifndef IDENTIFIER_SYMBOL_TABLE_H
#define IDENTIFIER_SYMBOL_TABLE_H

#include <unordered_map>
#include <string>
#include <llvm/IR/Function.h>

llvm::Value* find_identifier(const std::string &name);
void add_identifier(const std::string &name, llvm::Value *value);

#endif // IDENTIFIER_SYMBOL_TABLE_H