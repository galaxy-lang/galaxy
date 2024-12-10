#include "backend/generator/statements/generate_function_declaration_stmt.hpp"
#include "backend/generator/statements/generate_stmt.hpp"
#include "backend/generator/expressions/generate_expr.hpp"
#include "backend/generator/types/generate_type.hpp"

llvm::Value* generate_function_declaration_stmt(FunctionNode *node, llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder, llvm::Module &Module) {
    llvm::Type *return_type = generate_type(nullptr, Context, node->type);

    // Create a vector of LLVM Types for the function parameters
    std::vector<llvm::Type*> param_types;
    
    for (size_t i = 0; i < node->parameters->parameter_count; ++i) {
        // Retrieve each parameter node
        AstNode *param_node = node->parameters->parameters[i];
        ParameterNode *param = static_cast<ParameterNode*>(param_node->data);
        
        // Generate the type for each parameter and add it to the param_types vector
        llvm::Type* param_type = generate_type(nullptr, Context, param->type);
        param_types.push_back(param_type);
    }

    // Create the function type (specifying return type and parameter types)
    llvm::FunctionType *func_type = llvm::FunctionType::get(return_type, param_types, false);

    // Create the function in the LLVM module with external linkage (the function's symbol is externally visible)
    llvm::Function *function = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, node->name, &Module);

    // Set the names of the function parameters based on the parameter nodes
    size_t idx = 0;
    for (auto &arg : function->args()) {
        AstNode *param_node = node->parameters->parameters[idx];
        ParameterNode *param = static_cast<ParameterNode*>(param_node->data);
        // Set the name for the parameter argument
        arg.setName(param->name);
        ++idx;
    }

    // If the function has a body, create the corresponding LLVM IR for the function body
    if (node->body != nullptr) {
        // Create the entry block for the function (this is where instructions will be inserted)
        llvm::BasicBlock *entry = llvm::BasicBlock::Create(Context, "entry", function);
        Builder.SetInsertPoint(entry);  // Set the builder to insert instructions into the entry block

        // Generate the statements in the function body
        for (size_t i = 0; i < node->body_count; ++i) {
            generate_stmt(node->body[i], Context, Module, Builder);  // Generate each statement
        }

        // If the function has a void return type and the body doesn't end with a return statement, add a return statement
        if (return_type->isVoidTy() && Builder.GetInsertBlock()->getTerminator() == nullptr) {
            Builder.CreateRetVoid();  // Create a return with no value (void)
        }
    }

    // Return the generated function (LLVM Function object)
    return function;
}
