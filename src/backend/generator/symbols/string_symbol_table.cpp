#include "backend/generator/symbols/string_symbol_table.hpp"

std::unordered_map<std::string, llvm::Value *> string_symbol_table;

void add_string(const std::string &name, llvm::Value *value) {
    string_symbol_table[name] = value;
}

llvm::Value *find_string(const std::string &name) {
    auto it = string_symbol_table.find(name);
    if (it != string_symbol_table.end()) {
        return it->second;
    }
    return nullptr;
}