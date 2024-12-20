#include "backend/generator/expressions/generate_identifier.hpp"
#include "backend/generator/symbols/identifier_symbol_table.hpp"
#include "backend/generator/parallel/queue.hpp"

llvm::Value *generate_identifier(IdentifierNode *node) {
    try {
        return find_or_wait_for_identifier(node, global_id_return);
    } catch (...) {
        throw std::runtime_error("Error: identifier not found!");
    }
}

