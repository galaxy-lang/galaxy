#include "backend/generator/statements/generate_function_declaration_stmt.hpp"
#include "backend/generator/statements/generate_stmt.hpp"
#include "backend/generator/symbols/identifier_symbol_table.hpp"
#include "backend/generator/symbols/function_symbol_table.hpp"
#include "backend/generator/symbols/symbol_stack.hpp"
#include "backend/generator/types/generate_type.hpp"
#include "backend/generator/expressions/generate_expr.hpp"

llvm::Value* generate_function_declaration_stmt(FunctionNode *node, llvm::LLVMContext &Context, llvm::IRBuilder<llvm::NoFolder> &Builder, llvm::Module &Module) {
    if (!node || !node->name || !node->parameters) {
        throw std::runtime_error("Invalid function: node, name, or parameters are null.");
    }

    // Verifica se a função é a "main"
    bool is_main = (strcmp(node->name, "main") == 0);
    bool has_correct_main_params = false;

    if (is_main) {
        // Verifica se há exatamente dois parâmetros
        if (node->parameters->parameter_count == 2) {
            ParameterNode *param1 = static_cast<ParameterNode*>(node->parameters->parameters[0]->data);
            ParameterNode *param2 = static_cast<ParameterNode*>(node->parameters->parameters[1]->data);

            // Verifica se o primeiro parâmetro é string[] e o segundo é int
            if (strcmp(param1->type, "string") == 0 && param1->isPtr && // string[] é um ponteiro
                strcmp(param2->type, "int") == 0 && !param2->isPtr) {   // int não é um ponteiro
                has_correct_main_params = true;
            }
        }
    }

    // Gera o tipo de retorno da função
    llvm::Type *return_type = generate_type(node->type, Context);

    // Gera os tipos dos parâmetros
    std::vector<llvm::Type*> param_types;

    if (is_main && has_correct_main_params) {
        // Define os tipos corretos para main: i8** (argv) e i32 (argc)
        param_types.push_back(llvm::Type::getInt8Ty(Context)->getPointerTo()); // argv (char**)
        param_types.push_back(llvm::Type::getInt32Ty(Context));                   // argc (int)
    } else {
        // Gera os tipos normalmente para outras funções
        for (int i = 0; i < node->parameters->parameter_count; ++i) {
            ParameterNode *param = static_cast<ParameterNode*>(node->parameters->parameters[i]->data);
            llvm::Type *param_type = generate_type(param->type, Context);
            if (param->isPtr) {
                param_type = param_type->getPointerTo(); // Converte para ponteiro se necessário
            }
            param_types.push_back(param_type);
        }
    }

    // Cria o tipo da função no LLVM
    llvm::FunctionType *func_type = llvm::FunctionType::get(return_type, param_types, false);
    llvm::Function *function = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, node->name, &Module);

    // Armazena a função na tabela de símbolos
    function_symbol_table[node->name] = function;

    // Entra em um novo escopo para os parâmetros
    enter_scope();
    int idx = 0;

    if (is_main && has_correct_main_params) {
        // Processa os argumentos da função main
        int arg_idx = 0;
        for (auto &arg : function->args()) {
            ParameterNode *param = static_cast<ParameterNode*>(node->parameters->parameters[arg_idx]->data);
            arg.setName(param->name);
            // Usa o tipo real do argumento do LLVM (i8** ou i32)
            add_identifier(param->name, &arg, nullptr, arg.getType());
            arg_idx++;
        }
    } else {
        // Processa os argumentos para outras funções
        for (auto &arg : function->args()) {
            ParameterNode *param = static_cast<ParameterNode*>(node->parameters->parameters[idx]->data);
            arg.setName(param->name);
            llvm::Type *type = generate_type(param->type, Context);
            if (param->isPtr) {
                type = type->getPointerTo(); // Converte para ponteiro se necessário
            }
            add_identifier(param->name, &arg, nullptr, type);
            idx++;
        }
    }

    // Gera o corpo da função, se existir
    if (node->body != nullptr) {
        llvm::BasicBlock *entry = llvm::BasicBlock::Create(Context, "entry", function);
        Builder.SetInsertPoint(entry);

        std::map<std::string, llvm::Value*> variable_map;
        idx = 0;

        // Aloca espaço para os argumentos na pilha
        for (auto &arg : function->args()) {
            llvm::AllocaInst *alloca = Builder.CreateAlloca(arg.getType(), nullptr, arg.getName());
            Builder.CreateStore(&arg, alloca);
            variable_map[arg.getName().str()] = alloca;
        }

        // Gera as instruções do corpo da função
        for (size_t i = 0; i < node->body_count; ++i) {
            AstNode *statement = node->body[i];
            generate_stmt(statement, Context, Module, Builder); // Usa generate_stmt para gerar o corpo
        }

        // Garante que a função tenha um retorno válido
        if (return_type->isVoidTy()) {
            Builder.CreateRetVoid();
        } else {
            Builder.CreateRet(llvm::Constant::getNullValue(return_type));
        }
    }

    // Sai do escopo
    exit_scope();

    return function;
}