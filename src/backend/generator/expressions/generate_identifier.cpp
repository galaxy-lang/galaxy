#include "backend/generator/expressions/generate_identifier.hpp"
#include "backend/generator/symbols/identifier_symbol_table.hpp"
#include "backend/generator/utils/return_id.hpp"

llvm::Value *generate_identifier(IdentifierNode *node) {
    const SymbolInfo* id = find_identifier(node->symbol);
    if (id) {
        if (global_id_return == "declaration") {
            return id->declaration;
        } else {
            return id->value;
        }
    }
}