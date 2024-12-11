#include "backend/generator/symbols/identifier_symbol_table.hpp"
#include "backend/generator/symbols/symbol_stack.hpp"

llvm::Value* find_identifier(const std::string &name) {
    for (const auto& entry : symbol_stack.top()) {
        if (entry.first == name) {  // A chave é 'entry.first'
            return entry.second;    // O valor é 'entry.second'
        }
    }
    return nullptr; // Not found
}

void add_identifier(const std::string &name, llvm::Value *value) {
    if (symbol_stack.empty()) {
        throw std::runtime_error("Error: No active scope to add identifier.");
    }
    symbol_stack.top()[name] = value;
}