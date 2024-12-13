#include "backend/generator/expressions/generate_string.hpp"

llvm::Value *generate_string_literal(StringNode *string_node, llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder, llvm::Module &Module) {
    // Ensure that the StringNode and the string data are valid
    if (!string_node || !string_node->string) {
        throw std::runtime_error("Invalid string node: missing string value.");
    }

    // This represents the string as a constant array of characters in LLVM
    llvm::Constant *string_constant = llvm::ConstantDataArray::getString(Context, string_node->string, true);

    // This global variable will hold the constant array
    llvm::GlobalVariable *global_string = new llvm::GlobalVariable(
        Module, // The module to which this global variable belongs
        string_constant->getType(), // The type of the string constant (constant array of chars)
        true, // Whether the variable is constant
        llvm::GlobalValue::PrivateLinkage, // The linkage type (private to the module)
        string_constant, // The actual constant value of the string
        ".str" // The name of the global variable (used for debugging and identification)
    );

    // We use `getGetElementPtr` to get a pointer to the start of the array (the first character)
    llvm::Constant *zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 0); // The index for the first element of the array
    llvm::Constant *indices[] = {zero, zero}; // Indices for the GetElementPtr operation
    llvm::Constant *string_ptr = llvm::ConstantExpr::getGetElementPtr(
        string_constant->getType(), // The type of the string constant (constant array)
        global_string, // The global variable holding the string
        indices // The indices to access the first character
    );

    // This pointer can now be used wherever a pointer to the string is needed in the generated IR
    return string_ptr;
}
