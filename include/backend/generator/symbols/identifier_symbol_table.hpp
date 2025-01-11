#ifndef IDENTIFIER_SYMBOL_TABLE_H
#define IDENTIFIER_SYMBOL_TABLE_H

#include <unordered_map>
#include <string>
#include <llvm/IR/Function.h>
#include "backend/generator/symbols/symbol_stack.hpp"

SymbolInfo *find_identifier(const std::string &name);
void add_identifier(const std::string &name, llvm::Value* declaration, llvm::Value* value, llvm::Type* type);

#endif // IDENTIFIER_SYMBOL_TABLE_H