#include "backend/generator/expressions/generate_string.hpp"
#include "backend/generator/symbols/string_symbol_table.hpp"  // Inclua o header da tabela de símbolos de strings

llvm::Value *generate_string_literal(StringNode *string_node, llvm::LLVMContext &Context, llvm::IRBuilder<llvm::NoFolder> &Builder, llvm::Module &Module) {
    // Ensure that the StringNode and the string data are valid
    if (!string_node || !string_node->string) {
        throw std::runtime_error("Invalid string node: missing string value.");
    }

    // Check if the string is already in the symbol table
    llvm::Value *existing_string = find_string(string_node->string);
    if (existing_string) {
        return existing_string;  // Return the existing string if found
    }
    // Create a constant representing the string, including the null terminator
    llvm::Constant *string_constant = llvm::ConstantDataArray::getString(Context, string_node->string, true);

    // Allocate space on the stack for the string (including the null terminator)
    llvm::AllocaInst *alloca_inst = Builder.CreateAlloca(
        llvm::ArrayType::get(llvm::Type::getInt8Ty(Context), strlen(string_node->string) + 1), // +1 for the null terminator
        nullptr, // No specific alignment type
        "string_alloca"
    );

    // Use `store` to copy the string data into the allocated memory
    Builder.CreateStore(string_constant, alloca_inst);

    // Now, we need a pointer to the first character of the string in the stack memory
    // We use `CreateGEP` to get the pointer to the start of the array (the first character)
    llvm::Value *string_ptr = Builder.CreateGEP(
        alloca_inst->getAllocatedType(), // The type of the allocated array
        alloca_inst,                     // The pointer to the allocated memory
        {llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 0)}, // Index 0 to get the first character
        "string_ptr"
    );

    // Add the new string to the symbol table
    add_string(string_ptr->getName().str(), string_ptr);

    // Return the pointer to the string
    return string_ptr;
}
