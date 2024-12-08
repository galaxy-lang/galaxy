#include <vector>
#include "backend/generator/generate_ir.hpp"
#include "backend/generator/statements/generate_stmt.hpp"

std::vector<llvm::Value*> generate_ir(AstNode *node, llvm::LLVMContext &Context, llvm::Module &Module, llvm::IRBuilder<> &Builder) {
    if (node->kind != NODE_PROGRAM) {
        throw std::runtime_error("Expected program node");
    }

    ProgramNode *program = (ProgramNode *)node->data;
    std::vector<llvm::Value*> IRs;

    for (size_t i = 0; i < program->statement_count; ++i) {
        llvm::Value *IR = generate_stmt(program->statements[i], Context, Module, Builder);
        
        if (IR) {
            IRs.push_back(IR);
        }
    }

    return IRs;
}
