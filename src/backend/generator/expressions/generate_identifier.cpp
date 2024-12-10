#include "backend/generator/expressions/generate_identifier.hpp"

// A map that associates variable names (symbols) with their corresponding LLVM values.
std::map<std::string, llvm::Value *> NamedValues;

llvm::Value *generate_identifier(IdentifierNode *node) {
    // Search for the identifier in the NamedValues map using the symbol name
    auto it = NamedValues.find(node->symbol);
    
    // If the identifier is not found in the map
    if (it == NamedValues.end()) {
        // TODO: Handle the case where the identifier is not found (e.g., variable not declared)
    }

    // Return the LLVM Value associated with the found identifier
    return it->second; 
}
