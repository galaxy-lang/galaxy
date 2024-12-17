#include "backend/generator/symbols/identifier_symbol_table.hpp"
#include "backend/generator/symbols/symbol_stack.hpp"

const SymbolInfo *find_identifier(const std::string &name) {
    std::lock_guard<std::mutex> lock(symbol_stack_mutex);

    for (const auto& entry : symbol_stack.top()) {
        if (entry.first == name) {
            return &entry.second;
        }
    }
    
    return nullptr;
}

void add_identifier(const std::string& name, llvm::Value* value, llvm::Type* type) {
    if (symbol_stack.empty()) {
        throw std::runtime_error("No active scope to add identifier.");
    }
    symbol_stack.top()[name] = SymbolInfo{value, type};
}