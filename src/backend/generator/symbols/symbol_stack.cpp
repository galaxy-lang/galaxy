#include "backend/generator/symbols/symbol_stack.hpp"
#include "backend/generator/symbols/identifier_symbol_table.hpp"

std::stack<SymbolTable> symbol_stack;
std::mutex symbol_stack_mutex;

void enter_scope() {
    std::lock_guard<std::mutex> lock(symbol_stack_mutex);
    symbol_stack.push(SymbolTable{});
}

void exit_scope() {
    std::lock_guard<std::mutex> lock(symbol_stack_mutex);
    if (symbol_stack.empty()) {
        throw std::runtime_error("Exiting scope when no scope exists!");
    }
    symbol_stack.pop();
}