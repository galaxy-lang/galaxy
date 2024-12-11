#include "backend/generator/symbols/symbol_stack.hpp"

std::stack<SymbolTable> symbol_stack;


void enter_scope() {
    // Adiciona uma nova tabela de símbolos ao topo da pilha
    symbol_stack.push(SymbolTable{});
}

void exit_scope() {
    // Remove a tabela de símbolos do topo da pilha
    if (symbol_stack.empty()) {
        throw std::runtime_error("Exiting scope when no scope exists!");
    }
    symbol_stack.pop();
}