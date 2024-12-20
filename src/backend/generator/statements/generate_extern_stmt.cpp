#include "backend/generator/statements/generate_extern_stmt.hpp"
#include "backend/generator/types/generate_type.hpp"
#include "backend/generator/symbols/function_symbol_table.hpp"
#include "backend/generator/symbols/identifier_symbol_table.hpp"
#include "backend/generator/parallel/queue.hpp"

llvm::Value* generate_extern_stmt(ExternNode *node, llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder, llvm::Module &Module) {
    if (!node || !node->type || !node->identifier) {
        throw std::runtime_error("Invalid extern declaration.");
    }

    // Generate the type for the declaration
    llvm::Type *decl_type = generate_type(node->type, Context);
    if (!decl_type) {
        throw std::runtime_error("Failed to generate type for extern declaration.");
    }

    if (strcmp(node->extern_type, "function") == 0) {
        // Handle function declaration
        std::vector<llvm::Type *> param_types;
        for (size_t i = 0; i < node->arg_count; ++i) {
            llvm::Type *arg_type = generate_type(node->args[i], Context);
            if (!arg_type) {
                throw std::runtime_error("Failed to generate argument type for extern function.");
            }
            param_types.push_back(arg_type);
        }

        llvm::FunctionType *func_type = llvm::FunctionType::get(decl_type, param_types, false); // false for no varargs
        llvm::Function *function = llvm::Function::Create(
            func_type,
            llvm::Function::ExternalLinkage,
            node->identifier,
            &Module
        );

        function_symbol_table[node->identifier] = function;

        return function;
    } else if (strcmp(node->extern_type, "variable") == 0) {
        // Handle variable declaration
        llvm::GlobalVariable *global_var = llvm::cast<llvm::GlobalVariable>(
            Module.getOrInsertGlobal(node->identifier, decl_type)
        );

        global_var->setLinkage(llvm::GlobalValue::ExternalLinkage);

        {
            std::unique_lock<std::mutex> lock(symbolTableMutex);
            add_identifier(node->identifier, global_var, nullptr, decl_type);
        }
        
        return global_var;
    } else {
        throw std::runtime_error("Unsupported extern type: " + std::string(node->extern_type));
    }
}
