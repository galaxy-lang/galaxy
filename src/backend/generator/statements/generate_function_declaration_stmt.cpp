#include "backend/generator/statements/generate_function_declaration_stmt.hpp"
#include "backend/generator/statements/generate_stmt.hpp"
#include "backend/generator/expressions/generate_expr.hpp"

llvm::Value* generate_function_declaration_stmt(FunctionNode *node, llvm::LLVMContext &Context, llvm::Module &Module, llvm::IRBuilder<> &Builder) {
    if (!node || !node->name || !node->parameters) {
        throw std::runtime_error("Função inválida: nó, nome ou parâmetros nulos.");
    }

    // Determinar o tipo de retorno da função
    llvm::Type *return_type;
    switch (node->type) {
        case TYPE_INT: return_type = llvm::Type::getInt32Ty(Context); break;
        case TYPE_FLOAT: return_type = llvm::Type::getFloatTy(Context); break;
        case TYPE_DOUBLE: return_type = llvm::Type::getDoubleTy(Context); break;
        case TYPE_BOOL: return_type = llvm::Type::getInt1Ty(Context); break;
        case TYPE_VOID: return_type = llvm::Type::getVoidTy(Context); break;
        default: throw std::runtime_error("Tipo de retorno não suportado");
    }

    // Determinar os tipos dos parâmetros
    std::vector<llvm::Type*> param_types;
    for (size_t i = 0; i < node->parameters->parameter_count; ++i) {
        AstNode *param_node = node->parameters->parameters[i];
        ParameterNode *param = static_cast<ParameterNode*>(param_node->data);
        switch (param->type) {
            case TYPE_INT: param_types.push_back(llvm::Type::getInt32Ty(Context)); break;
            case TYPE_FLOAT: param_types.push_back(llvm::Type::getFloatTy(Context)); break;
            case TYPE_DOUBLE: param_types.push_back(llvm::Type::getDoubleTy(Context)); break;
            case TYPE_BOOL: param_types.push_back(llvm::Type::getInt1Ty(Context)); break;
            default: throw std::runtime_error("Tipo de parâmetro não suportado");
        }
    }

    // Criar o tipo da função
    llvm::FunctionType *func_type = llvm::FunctionType::get(return_type, param_types, false);

    // Adicionar a função ao módulo
    llvm::Function *function = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, node->name, &Module);

    // Definir os nomes dos parâmetros
    size_t idx = 0;
    for (auto &arg : function->args()) {
        AstNode *param_node = node->parameters->parameters[idx];
        ParameterNode *param = static_cast<ParameterNode*>(param_node->data);
        arg.setName(param->name);
        ++idx;
    }

    // Criar o corpo da função, se disponível
    if (node->body != nullptr) {
        llvm::BasicBlock *entry = llvm::BasicBlock::Create(Context, "entry", function);
        Builder.SetInsertPoint(entry);

        // Processar o corpo da função
        for (size_t i = 0; i < node->body_count; ++i) {
            generate_stmt(node->body[i], Context, Module, Builder);
        }

        // Adicionar retorno void se a função for do tipo void e não houver retorno explícito
        if (return_type->isVoidTy() && Builder.GetInsertBlock()->getTerminator() == nullptr) {
            Builder.CreateRetVoid();
        }
    }

    return function;
}
