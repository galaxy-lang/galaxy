#include "backend/generator/expressions/generate_call.hpp"
#include "backend/generator/expressions/generate_expr.hpp"
#include "backend/generator/symbols/function_symbol_table.hpp"
#include "backend/generator/utils/return_id.hpp"
#include "backend/generator/parallel/queue.hpp"
#include "backend/generator/symbols/iterator_stack.hpp"
#include <iostream>

llvm::Value *generate_call(CallNode *call_node, llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder, llvm::Module &Module) {
    if (!call_node || !call_node->caller) {
        throw std::runtime_error("Invalid call expression: missing caller.");
    }

    // Assume the caller is a function name (e.g., NODE_IDENTIFIER)
    IdentifierNode *identifier = static_cast<IdentifierNode *>(call_node->caller->data);
    const std::string &function_name = identifier->symbol;

    // Lookup the function in the symbol table
    auto it = function_symbol_table.find(function_name);
    if (it == function_symbol_table.end()) {
        throw std::runtime_error("Function not found in symbol table: " + function_name);
    }

    llvm::Function *function = it->second;
    llvm::FunctionType *func_type = function->getFunctionType();

    // Generate arguments
    std::vector<llvm::Value *> args;
    for (size_t i = 0; i < call_node->arg_count; ++i) {
        global_id_return = "value";

        // Declare expected_type here before using it
        llvm::Type* expected_type = func_type->getParamType(i);

        // Get the current iterator info
        IteratorInfo* current_iter = current_iterator();
        llvm::Value *arg = nullptr;

        if (current_iter) {
            // Get the current argument as an IdentifierNode
            IdentifierNode* arg_identifier = static_cast<IdentifierNode*>(call_node->args[i]->data);
            if (arg_identifier && current_iter->value && current_iter->value->getName().str() == arg_identifier->symbol) {
                // If the current iterator matches the argument name, use the current iterator data
                arg = current_iter->value;
                expected_type = current_iter->type; // Override expected_type with current iterator's type
            }
        }


        if (!arg) {
            // Otherwise, generate the argument value as usual
            global_id_return = "declaration";
            arg = generate_expr(call_node->args[i], Context, Builder, Module);
            if (!arg) {
                throw std::runtime_error("Failed to generate call argument.");
            }
        }

        llvm::errs() << *arg << "\n";
        // Check if the argument is a pointer
        if (arg->getType()->isPointerTy()) {
            if (expected_type->isPointerTy()) {
                args.push_back(arg);
            } else {
                llvm::Value *loaded_value = Builder.CreateLoad(expected_type, arg);
                args.push_back(loaded_value);
            }
        } else {
            if (arg->getType() != expected_type) {
                if (arg->getType()->isIntegerTy() && expected_type->isIntegerTy()) {
                    arg = Builder.CreateIntCast(arg, expected_type, true);
                } else if (arg->getType()->isFloatingPointTy() && expected_type->isFloatingPointTy()) {
                    arg = Builder.CreateFPCast(arg, expected_type);
                } else if (arg->getType()->isIntegerTy() && expected_type->isFloatingPointTy()) {
                    arg = Builder.CreateSIToFP(arg, expected_type);
                } else if (arg->getType()->isFloatingPointTy() && expected_type->isIntegerTy()) {
                    arg = Builder.CreateFPToSI(arg, expected_type);
                } else {
                    throw std::runtime_error("Argument type mismatch.");
                }
            }
            args.push_back(arg);
        }
    }

    return Builder.CreateCall(function, args);
}
