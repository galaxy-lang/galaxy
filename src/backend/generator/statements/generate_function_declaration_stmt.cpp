#include "backend/generator/statements/generate_function_declaration_stmt.hpp"
#include "backend/generator/statements/generate_stmt.hpp"
#include "backend/generator/expressions/generate_expr.hpp"
#include "backend/generator/types/generate_type.hpp"
#include "backend/generator/symbols/function_symbol_table.hpp"
#include "backend/generator/symbols/symbol_stack.hpp"
#include "backend/generator/symbols/identifier_symbol_table.hpp"

llvm::Value* generate_function_declaration_stmt(FunctionNode *node, llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder, llvm::Module &Module) {
    if (!node || !node->name || !node->parameters) {
        throw std::runtime_error("Invalid function: node, name, or parameters are null.");
    }


    // Generate the return type for the function
    llvm::Type *return_type = generate_type(nullptr, Context, node->type);

    // Generate parameter types
    std::vector<llvm::Type*> param_types;
    for (int i = 0; i < node->parameters->parameter_count; ++i) {
        ParameterNode *param = static_cast<ParameterNode*>(node->parameters->parameters[i]->data);
        llvm::Type *param_type = generate_type(nullptr, Context, param->type);
        param_types.push_back(param_type);
    }

    // Create the function type and function
    llvm::FunctionType *func_type = llvm::FunctionType::get(return_type, param_types, false);
    llvm::Function *function = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, node->name, &Module);

    // Saves the function on a symbol table
    function_symbol_table[node->name] = function;
    
    // Assign parameter names
    enter_scope();
    int idx = 0;
    for (auto &arg : function->args()) {
        ParameterNode *param = static_cast<ParameterNode*>(node->parameters->parameters[idx]->data);
        arg.setName(param->name);

        add_identifier(param->name, &arg); // Stores in the identifier symbol table
        ++idx;
    }

    if (node->body != nullptr) {
        llvm::BasicBlock *entry = llvm::BasicBlock::Create(Context, "entry", function);
        Builder.SetInsertPoint(entry);

        // Map function parameters to LLVM variables
        std::map<std::string, llvm::Value*> variable_map;
        idx = 0;
        for (auto &arg : function->args()) {
            llvm::AllocaInst *alloca = Builder.CreateAlloca(arg.getType(), nullptr, arg.getName());
            Builder.CreateStore(&arg, alloca);
            variable_map[arg.getName().str()] = alloca;
        }

        llvm::Value *return_value = nullptr;

        for (size_t i = 0; i < node->body_count; ++i) {
            AstNode *statement = node->body[i];

            if (statement->kind == NODE_RETURN) {
                ReturnNode *return_node = static_cast<ReturnNode*>(statement->data);
                AstNode *value_node = return_node->value;

                // Generate the return expression
                return_value = generate_expr(value_node, Context, Builder, Module);

                if (!return_value) {
                    llvm::errs() << "Error: generate_expr returned null for return expression!\n";
                    if (return_type->isVoidTy()) {
                        Builder.CreateRetVoid();
                    } else {
                        return_value = llvm::Constant::getNullValue(return_type);
                        Builder.CreateRet(return_value);
                        return function;
                    }
                } else {
                    Builder.CreateRet(return_value);
                    return function;
                }

            } else {
                // Handle other statement types
                generate_stmt(statement, Context, Module, Builder);
            }
        }

        // Add a default return if no explicit return is provided
        if (!return_value) {
            if (return_type->isVoidTy()) {
                Builder.CreateRetVoid();
            } else {
                Builder.CreateRet(llvm::Constant::getNullValue(return_type));
            }
        }
    }

    exit_scope();

    return function;
}
