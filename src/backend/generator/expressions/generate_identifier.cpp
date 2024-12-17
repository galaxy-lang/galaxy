#include "backend/generator/expressions/generate_identifier.hpp"
#include "backend/generator/symbols/identifier_symbol_table.hpp"

llvm::Value *generate_identifier(IdentifierNode *node) {
    const SymbolInfo *id = find_identifier(node->symbol);

    if (!id) {
        throw std::runtime_error("Error: identifier not found!");
    }

    return id->value;
}
