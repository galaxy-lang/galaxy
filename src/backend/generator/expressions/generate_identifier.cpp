#include "backend/generator/expressions/generate_identifier.hpp"
#include "backend/generator/symbols/identifier_symbol_table.hpp"

llvm::Value *generate_identifier(IdentifierNode *node) {
    // Usa find_identifier para buscar o valor associado ao símbolo no escopo atual
    llvm::Value *value = find_identifier(node->symbol);

    // Verifica se o identificador foi encontrado
    if (!value) {
        throw std::runtime_error("Error: identifier not found!");
    }

    // Retorna o LLVM Value associado ao identificador
    return value;
}
