#ifndef STRING_SYMBOL_TABLE_H
#define STRING_SYMBOL_TABLE_H

#include <unordered_map>
#include <string>
#include <llvm/IR/Value.h>

extern std::unordered_map<std::string, llvm::Value *> string_symbol_table;

void add_string(const std::string &name, llvm::Value *value);
llvm::Value *find_string(const std::string &name);

#endif // STRING_SYMBOL_TABLE_H