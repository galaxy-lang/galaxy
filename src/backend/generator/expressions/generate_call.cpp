#include "backend/generator/expressions/generate_call.hpp"
#include "backend/generator/expressions/generate_expr.hpp"
#include "backend/generator/symbols/function_symbol_table.hpp"

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
        llvm::Value *arg = generate_expr(call_node->args[i], Context, Builder, Module);
        if (!arg) {
            throw std::runtime_error("Failed to generate call argument.");
        }

        // Ensure the argument type matches the function parameter type
        llvm::Type *expected_type = func_type->getParamType(i);
        if (arg->getType() != expected_type) {
            if (arg->getType()->isIntegerTy() && expected_type->isIntegerTy()) {
                arg = Builder.CreateIntCast(arg, expected_type, true);
            } else if (arg->getType()->isFloatingPointTy() && expected_type->isFloatingPointTy()) {
                arg = Builder.CreateFPCast(arg, expected_type);
            } else if (arg->getType()->isPointerTy() && expected_type->isIntegerTy()) {
                arg = Builder.CreatePointerCast(arg, expected_type);
            } else if (arg->getType()->isPointerTy() && expected_type->isFloatingPointTy()) {
                arg = Builder.CreatePointerCast(arg, expected_type);
            } else {
                throw std::runtime_error("Argument type mismatch.");
            }
        }

        args.push_back(arg);
    }

    // Create the call instruction
    return Builder.CreateCall(function, args);
}
