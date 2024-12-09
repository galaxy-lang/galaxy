#include "backend/generator/statements/generate_function_declaration_stmt.hpp"
#include "backend/generator/statements/generate_stmt.hpp"
#include "backend/generator/expressions/generate_expr.hpp"
#include "backend/generator/types/generate_type.hpp"

llvm::Value* generate_function_declaration_stmt(FunctionNode *node, llvm::LLVMContext &Context, llvm::Module &Module, llvm::IRBuilder<> &Builder) {
    if (!node || !node->name || !node->parameters) {
        throw std::runtime_error("Invalid function: Node and/or Parameters is/are NULL.");
    }

    llvm::Type *return_type = generate_type(nullptr, Context, node->type);

    std::vector<llvm::Type*> param_types;
    for (size_t i = 0; i < node->parameters->parameter_count; ++i) {
        AstNode *param_node = node->parameters->parameters[i];
        ParameterNode *param = static_cast<ParameterNode*>(param_node->data);
        
        llvm::Type* param_type = generate_type(nullptr, Context, param->type);
        param_types.push_back(param_type);
    }

    llvm::FunctionType *func_type = llvm::FunctionType::get(return_type, param_types, false);

    llvm::Function *function = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, node->name, &Module);

    size_t idx = 0;
    for (auto &arg : function->args()) {
        AstNode *param_node = node->parameters->parameters[idx];
        ParameterNode *param = static_cast<ParameterNode*>(param_node->data);
        arg.setName(param->name);
        ++idx;
    }

    if (node->body != nullptr) {
        llvm::BasicBlock *entry = llvm::BasicBlock::Create(Context, "entry", function);
        Builder.SetInsertPoint(entry);

        for (size_t i = 0; i < node->body_count; ++i) {
            generate_stmt(node->body[i], Context, Module, Builder);
        }

        if (return_type->isVoidTy() && Builder.GetInsertBlock()->getTerminator() == nullptr) {
            Builder.CreateRetVoid();
        }
    }

    return function;
}
