#include "backend/generator/expressions/generate_identifier.hpp"

std::map<std::string, llvm::Value *> NamedValues;

llvm::Value *generate_identifier(IdentifierNode *node) {
    auto it = NamedValues.find(node->symbol);
    if (it == NamedValues.end()) {
        throw std::runtime_error("Undefined variable: " + std::string(node->symbol));
    }
    return it->second; 
}
