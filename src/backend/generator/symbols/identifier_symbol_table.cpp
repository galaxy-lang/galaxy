#include "backend/generator/symbols/identifier_symbol_table.hpp"
#include "backend/generator/symbols/symbol_stack.hpp"
#include <stdexcept>

// Função para encontrar o identificador na pilha de símbolos
SymbolInfo *find_identifier(const std::string &name) {
    if (symbol_stack.empty()) {
        throw std::runtime_error("Symbol stack is empty. Cannot find identifier.");
    }

    for (auto& entry : symbol_stack.top()) {
        if (entry.first == name) {
            return &entry.second;
        }
    }

    return nullptr;
}

// Função para adicionar um identificador à pilha de símbolos
void add_identifier(const std::string& name, llvm::Value* declaration, llvm::Value* value, llvm::Type* type) {
    if (symbol_stack.empty()) {
        throw std::runtime_error("No active scope to add identifier.");
    }

    // Adiciona o identificador à pilha de símbolos
    symbol_stack.top()[name] = SymbolInfo{declaration, value, type};
}
