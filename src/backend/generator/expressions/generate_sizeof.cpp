#include "backend/generator/expressions/generate_sizeof.hpp"
#include "backend/generator/types/generate_type.hpp"

llvm::Value* generate_sizeof(SizeofNode *sizeofNode, llvm::LLVMContext &Context, llvm::IRBuilder<llvm::NoFolder> &Builder, llvm::Module &Module) {
    // Generate the LLVM type for the expression
    llvm::Type* type = generate_type(sizeofNode->expression, false, Context);

    // Get the size of the type
    llvm::DataLayout dataLayout(&Module);
    uint64_t typeSize = dataLayout.getTypeAllocSize(type);

    // Return the size as a 64-bit integer constant
    llvm::Constant* size = llvm::ConstantInt::get(llvm::Type::getInt64Ty(Context), typeSize);

    return size;
}