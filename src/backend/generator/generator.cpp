#include <vector>
#include "backend/generator/generate_ir.hpp"
#include "backend/generator/statements/generate_stmt.hpp"

std::vector<llvm::Value*> generate_ir(AstNode *node, llvm::LLVMContext &Context, llvm::Module &Module, llvm::IRBuilder<> &Builder) {
    // Cast the AstNode to ProgramNode as the data in AstNode is assumed to be a ProgramNode
    ProgramNode *program = (ProgramNode *)node->data;

    // Vector to hold the generated LLVM IR values
    std::vector<llvm::Value*> IRs;

    // Iterate over all statements in the program and generate their corresponding IR
    for (size_t i = 0; i < program->statement_count; ++i) {
        // Generate LLVM IR for each statement
        llvm::Value *IR = generate_stmt(program->statements[i], Context, Module, Builder);
        
        // If the generated IR is not null, add it to the result vector
        if (IR) {
            IRs.push_back(IR);
        }
    }

    // Return the vector containing the generated IR for the statements
    return IRs;
}
