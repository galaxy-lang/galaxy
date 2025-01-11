#include "backend/generator/symbols/iterator_stack.hpp"
#include <stdexcept>

std::stack<IteratorInfo> iterator_stack;
std::mutex iterator_stack_mutex;

void push_iterator(llvm::Value* declaration, llvm::Value* value, llvm::Type* type) {
    std::lock_guard<std::mutex> lock(iterator_stack_mutex);
    iterator_stack.push(IteratorInfo{declaration, value, type});
}

IteratorInfo pop_iterator() {
    std::lock_guard<std::mutex> lock(iterator_stack_mutex);
    if (iterator_stack.empty()) {
        throw std::runtime_error("Trying to pop from an empty iterator stack!");
    }
    IteratorInfo top_info = iterator_stack.top();
    iterator_stack.pop();
    return top_info;
}

IteratorInfo* current_iterator() {
    std::lock_guard<std::mutex> lock(iterator_stack_mutex);
    if (iterator_stack.empty()) {
        return nullptr;
    }
    return &iterator_stack.top();
}
