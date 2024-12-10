#include "backend/generator/expressions/generate_binary_expr.hpp"
#include "backend/generator/expressions/generate_expr.hpp"

llvm::Value *generate_binary_expr(BinaryExprNode *node, llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder, llvm::Module &Module) {
    // Generate LLVM IR for the left-hand side and right-hand side expressions
    llvm::Value *L = generate_expr(node->left, Context, Builder, Module);
    llvm::Value *R = generate_expr(node->right, Context, Builder, Module);

    // Handle the addition operation: Create an LLVM FAdd instruction (floating-point addition)
    if (strcmp(node->op, "+") == 0) return Builder.CreateFAdd(L, R, "addtmp");

    // Handle the subtraction operation: Create an LLVM FSub instruction (floating-point subtraction)
    if (strcmp(node->op, "-") == 0) return Builder.CreateFSub(L, R, "subtmp");

    // Handle the multiplication operation: Create an LLVM FMul instruction (floating-point multiplication)
    if (strcmp(node->op, "*") == 0) return Builder.CreateFMul(L, R, "multmp");

    // Handle the division operation: Check for division by zero
    if (strcmp(node->op, "/") == 0) {
        // Create a function to handle division by zero errors
        llvm::Function *divByZeroFunc = llvm::Function::Create(
            llvm::FunctionType::get(llvm::Type::getVoidTy(Context), false),  // Function type: void return, no parameters
            llvm::Function::ExternalLinkage,  // External linkage for the function (can be called from outside)
            "div_by_zero",  // Function name
            &Module  // Module to add the function to
        );

        // Generate a comparison: Check if the divisor (R) is zero
        llvm::Value *isZero = Builder.CreateFCmpOEQ(R, llvm::ConstantFP::get(Context, llvm::APFloat(0.0)), "is_zero");

        // Create two basic blocks: one for the error case, and one for continuing the division
        llvm::BasicBlock *errorBlock = llvm::BasicBlock::Create(Context, "error", divByZeroFunc);
        llvm::BasicBlock *continueBlock = llvm::BasicBlock::Create(Context, "continue", divByZeroFunc);

        // Create a conditional branch based on the result of the division by zero check
        Builder.CreateCondBr(isZero, errorBlock, continueBlock);

        // In the error block, call the division-by-zero handling function and return from the function
        Builder.SetInsertPoint(errorBlock);
        Builder.CreateCall(divByZeroFunc);  // Call the divByZero function (likely to halt or log the error)
        Builder.CreateRetVoid();  // Return from the function (terminates execution)

        // In the continue block, insert the division instruction
        Builder.SetInsertPoint(continueBlock);
        
        // Perform the floating-point division (FDiv)
        return Builder.CreateFDiv(L, R, "divtmp");
    }

    // Throw an exception if an unsupported operator is encountered
    throw std::runtime_error("Unknown binary operator");
}
