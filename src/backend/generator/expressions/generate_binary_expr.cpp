#include "backend/generator/expressions/generate_binary_expr.hpp"
#include "backend/generator/expressions/generate_expr.hpp"
#include "backend/generator/symbols/identifier_symbol_table.hpp"
#include "backend/generator/symbols/string_symbol_table.hpp"
#include "backend/generator/symbols/function_symbol_table.hpp"

llvm::Value *generate_binary_expr(BinaryExprNode *node, llvm::LLVMContext &Context, llvm::IRBuilder<llvm::NoFolder> &Builder, llvm::Module &Module) {
    llvm::Value *L = generate_expr(node->left, Context, Builder, Module);
    llvm::Value *R = generate_expr(node->right, Context, Builder, Module);

    llvm::Type *LType = L->getType();
    llvm::Type *RType = R->getType();

    bool isLInteger = LType->isIntegerTy();
    bool isRInteger = RType->isIntegerTy();
    bool isLFloating = LType->isFloatingPointTy();
    bool isRFloating = RType->isFloatingPointTy();
    bool isLPointer = LType->isPointerTy();
    bool isRPointer = RType->isPointerTy();

    if (isLPointer) {
        llvm::errs() << L->getValueName()->getValue()->getName().str() << "\n";
        try {
            llvm::Value *string = find_string(L->getValueName()->getValue()->getName().str());
            if (string) {
                if (isRInteger && strcmp(node->op, "*") == 0) {
                    auto it = function_symbol_table.find("strrep");
                    if (it == function_symbol_table.end()) {
                        throw std::runtime_error("Function not found in symbol table: strrep");
                    }

                    llvm::Function *function = it->second;
                    llvm::FunctionType *func_type = function->getFunctionType();

                    // Prepare arguments for the strrep function
                    std::vector<llvm::Value *> args;
                    // The first argument is the string (L)
                    args.push_back(string);
                    // The second argument is the repetition count (R)
                    args.push_back(R);

                    // Ensure the types of the arguments are correct
                    for (size_t i = 0; i < args.size(); ++i) {
                        llvm::Value *arg = args[i];
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
                        args[i] = arg;
                    }

                    // Call the strrep function
                    llvm::Value *resultBuffer = Builder.CreateCall(function, args);
                    return resultBuffer;
                }
            }
        } catch (...) {
            const SymbolInfo* symbolInfo = find_identifier(static_cast<IdentifierNode*>(node->left->data)->symbol);
            if (!symbolInfo) {
                throw std::runtime_error("Unsupported Left type for binary operation");
            }
            llvm::Type* pointeeType = symbolInfo->type;

            if (pointeeType->isIntegerTy()) {
                L = Builder.CreateLoad(pointeeType, L, "loadtmp");
                isLInteger = true;
            } else if (pointeeType->isFloatingPointTy()) {
                L = Builder.CreateLoad(pointeeType, L, "loadtmp");
                isLFloating = true;
            } else {
                throw std::runtime_error("Unsupported type for left pointer operand");
            }
        }
    }

    if (isRPointer) {
        const SymbolInfo* symbolInfo = find_identifier(static_cast<IdentifierNode*>(node->left->data)->symbol);
        if (!symbolInfo) {
            throw std::runtime_error("Unknown identifier for right operand");
        }
        llvm::Type* pointeeType = symbolInfo->type;

        if (pointeeType->isIntegerTy()) {
            R = Builder.CreateLoad(pointeeType, R, "loadtmp");
            isRInteger = true;
        } else if (pointeeType->isFloatingPointTy()) {
            R = Builder.CreateLoad(pointeeType, R, "loadtmp");
            isRFloating = true;
        } else {
            throw std::runtime_error("Unsupported type for right pointer operand");
        }
    }

    if (isLInteger && isRInteger) {
        if (strcmp(node->op, "+") == 0) return Builder.CreateAdd(L, R, "addtmp");
        if (strcmp(node->op, "-") == 0) return Builder.CreateSub(L, R, "subtmp");
        if (strcmp(node->op, "*") == 0) return Builder.CreateMul(L, R, "multmp");
        if (strcmp(node->op, "/") == 0) return Builder.CreateSDiv(L, R, "divtmp");
        if (strcmp(node->op, "%") == 0) return Builder.CreateSRem(L, R, "modtmp");
        if (strcmp(node->op, "&") == 0) return Builder.CreateAnd(L, R, "andtmp");
        if (strcmp(node->op, "|") == 0) return Builder.CreateOr(L, R, "ortmp");
        if (strcmp(node->op, "^") == 0) return Builder.CreateXor(L, R, "xortmp");
        if (strcmp(node->op, ">>") == 0) return Builder.CreateAShr(L, R, "shrtmp");
        if (strcmp(node->op, "<<") == 0) return Builder.CreateShl(L, R, "shltmp");

        if (strcmp(node->op, "==") == 0) return Builder.CreateICmpEQ(L, R, "eqtmp");
        if (strcmp(node->op, "!=") == 0) return Builder.CreateICmpNE(L, R, "netmp");
        if (strcmp(node->op, "<") == 0) return Builder.CreateICmpSLT(L, R, "lttmp");
        if (strcmp(node->op, "<=") == 0) return Builder.CreateICmpSLE(L, R, "letmp");
        if (strcmp(node->op, ">") == 0) return Builder.CreateICmpSGT(L, R, "gttmp");
        if (strcmp(node->op, ">=") == 0) return Builder.CreateICmpSGE(L, R, "getmp");

        throw std::runtime_error("Unknown binary operator for integers");
    }

    if (isLFloating || isRFloating) {
        if (isLInteger) {
            L = Builder.CreateSIToFP(L, llvm::Type::getDoubleTy(Context), "cast_to_fp_L");
        }
        if (isRInteger) {
            R = Builder.CreateSIToFP(R, llvm::Type::getDoubleTy(Context), "cast_to_fp_R");
        }

        if (strcmp(node->op, "+") == 0) return Builder.CreateFAdd(L, R, "addtmp");
        if (strcmp(node->op, "-") == 0) return Builder.CreateFSub(L, R, "subtmp");
        if (strcmp(node->op, "*") == 0) return Builder.CreateFMul(L, R, "multmp");
        if (strcmp(node->op, "/") == 0) return Builder.CreateFDiv(L, R, "divtmp");

        if (strcmp(node->op, "==") == 0) return Builder.CreateFCmpOEQ(L, R, "eqtmp");
        if (strcmp(node->op, "!=") == 0) return Builder.CreateFCmpONE(L, R, "netmp");
        if (strcmp(node->op, "<") == 0) return Builder.CreateFCmpOLT(L, R, "lttmp");
        if (strcmp(node->op, "<=") == 0) return Builder.CreateFCmpOLE(L, R, "letmp");
        if (strcmp(node->op, ">") == 0) return Builder.CreateFCmpOGT(L, R, "gttmp");
        if (strcmp(node->op, ">=") == 0) return Builder.CreateFCmpOGE(L, R, "getmp");

        throw std::runtime_error("Unsupported operator for floating-point numbers");
    }

    throw std::runtime_error("Unsupported types for binary operation");
}
