#include "backend/generator/statements/generate_variable_declaration_stmt.hpp"
#include "backend/generator/expressions/generate_expr.hpp"
#include "backend/generator/types/generate_type.hpp"
#include "backend/generator/symbols/identifier_symbol_table.hpp"

llvm::Value* generate_variable_declaration_stmt(VariableNode *node, llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder, llvm::Module &Module) {
    llvm::Type *var_type = generate_type(node->varType, Context);

    // Create an AllocaInst to allocate space for the variable on the stack
    llvm::AllocaInst *alloca = Builder.CreateAlloca(var_type, nullptr, node->name);

    if (node->value != nullptr) {
        // Generate IR for the expression
        llvm::Value *init_value = generate_expr(node->value, Context, Builder, Module);

        // Store the initialized value into the allocated space (AllocaInst)
        Builder.CreateStore(init_value, alloca);
    }

    // Stores the allocated variable in the identifier symbol table
    {
        std::lock_guard<std::mutex> lock(symbol_stack_mutex);
        add_identifier(node->name, alloca, var_type);
    }

    return alloca;
}
