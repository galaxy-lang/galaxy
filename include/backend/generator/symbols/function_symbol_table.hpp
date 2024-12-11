#ifndef FUNCTION_SYMBOL_TABLE_H
#define FUNCTION_SYMBOL_TABLE_H

#include <unordered_map>
#include <string>
#include <llvm/IR/Function.h>

extern std::unordered_map<std::string, llvm::Function *> function_symbol_table;

#endif // FUNCTION_SYMBOL_TABLE_H