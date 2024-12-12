#include "backend/generator/expressions/generate_binary_expr.hpp"
#include "backend/generator/expressions/generate_expr.hpp"

llvm::Value *generate_binary_expr(BinaryExprNode *node, llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder, llvm::Module &Module) {
    // Generate LLVM IR for the left-hand side and right-hand side expressions
    llvm::Value *L = generate_expr(node->left, Context, Builder, Module);
    llvm::Value *R = generate_expr(node->right, Context, Builder, Module);

    // Determine the types of L and R
    bool isLInteger = L->getType()->isIntegerTy();
    bool isRInteger = R->getType()->isIntegerTy();

    // If both are integers, handle integer-specific operations
    if (isLInteger && isRInteger) {
        if (strcmp(node->op, "+") == 0) return Builder.CreateAdd(L, R, "addtmp");
        if (strcmp(node->op, "-") == 0) return Builder.CreateSub(L, R, "subtmp");
        if (strcmp(node->op, "*") == 0) return Builder.CreateMul(L, R, "multmp");
        if (strcmp(node->op, "/") == 0) return Builder.CreateSDiv(L, R, "divtmp"); // Signed division
        if (strcmp(node->op, "%") == 0) return Builder.CreateSRem(L, R, "modtmp"); // Modulus
        if (strcmp(node->op, "&") == 0) return Builder.CreateAnd(L, R, "andtmp");
        if (strcmp(node->op, "|") == 0) return Builder.CreateOr(L, R, "ortmp");
        if (strcmp(node->op, "^") == 0) return Builder.CreateXor(L, R, "xortmp");
        if (strcmp(node->op, ">>") == 0) return Builder.CreateAShr(L, R, "shrtmp"); // Arithmetic shift right
        if (strcmp(node->op, "<<") == 0) return Builder.CreateShl(L, R, "shltmp"); // Shift left

        // Comparison operators
        if (strcmp(node->op, "==") == 0) return Builder.CreateICmpEQ(L, R, "eqtmp");
        if (strcmp(node->op, "!=") == 0) return Builder.CreateICmpNE(L, R, "netmp");
        if (strcmp(node->op, "<") == 0) return Builder.CreateICmpSLT(L, R, "lttmp");
        if (strcmp(node->op, "<=") == 0) return Builder.CreateICmpSLE(L, R, "letmp");
        if (strcmp(node->op, ">") == 0) return Builder.CreateICmpSGT(L, R, "gttmp");
        if (strcmp(node->op, ">=") == 0) return Builder.CreateICmpSGE(L, R, "getmp");

        throw std::runtime_error("Unknown binary operator for integers");
    }

    // If either is floating-point, cast both to floating-point and perform floating-point operations
    if (isLInteger) {
        L = Builder.CreateSIToFP(L, llvm::Type::getDoubleTy(Context), "cast_to_fp_L");
    }
    if (isRInteger) {
        R = Builder.CreateSIToFP(R, llvm::Type::getDoubleTy(Context), "cast_to_fp_R");
    }

    // Floating-point operations
    if (strcmp(node->op, "+") == 0) return Builder.CreateFAdd(L, R, "addtmp");
    if (strcmp(node->op, "-") == 0) return Builder.CreateFSub(L, R, "subtmp");
    if (strcmp(node->op, "*") == 0) return Builder.CreateFMul(L, R, "multmp");
    if (strcmp(node->op, "/") == 0) return Builder.CreateFDiv(L, R, "divtmp");

    // Comparison operators for floating-point
    if (strcmp(node->op, "==") == 0) return Builder.CreateFCmpOEQ(L, R, "eqtmp");
    if (strcmp(node->op, "!=") == 0) return Builder.CreateFCmpONE(L, R, "netmp");
    if (strcmp(node->op, "<") == 0) return Builder.CreateFCmpOLT(L, R, "lttmp");
    if (strcmp(node->op, "<=") == 0) return Builder.CreateFCmpOLE(L, R, "letmp");
    if (strcmp(node->op, ">") == 0) return Builder.CreateFCmpOGT(L, R, "gttmp");
    if (strcmp(node->op, ">=") == 0) return Builder.CreateFCmpOGE(L, R, "getmp");

    // Handle unsupported operators for floating-point
    throw std::runtime_error("Unsupported operator for floating-point numbers");
}
