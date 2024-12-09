#include "backend/generator/types/generate_type.hpp"

llvm::Type* generate_type(Parser *parser, llvm::LLVMContext &Context, const char* parsed_type) {
    if (strcmp(parsed_type, "int") == 0) {
        return llvm::Type::getInt32Ty(Context);
    } else if (strcmp(parsed_type, "float") == 0) {
        return llvm::Type::getFloatTy(Context);
    } else if (strcmp(parsed_type, "double") == 0) {
        return llvm::Type::getDoubleTy(Context);
    } else if (strcmp(parsed_type, "bool") == 0) {
        return llvm::Type::getInt1Ty(Context);
    } else if (strcmp(parsed_type, "void") == 0) {
        return llvm::Type::getVoidTy(Context);
    } else if (strcmp(parsed_type, "string") == 0) {
        return llvm::PointerType::get(llvm::Type::getInt8Ty(Context), 0);
    }

    else if (strncmp(parsed_type, "list", 4) == 0 || strncmp(parsed_type, "tuple", 5) == 0) {
        if (parsed_type[strlen(parsed_type) - 1] == '>') {
            size_t start_pos = strchr(parsed_type, '<') - parsed_type + 1;
            size_t len = strlen(parsed_type) - start_pos - 1;
            char* inner_type = strndup(parsed_type + start_pos, len);
            
            llvm::Type* element_type = generate_type(parser, Context, inner_type);
            free(inner_type);

            return llvm::PointerType::get(element_type, 0);
        }

        return llvm::PointerType::get(llvm::Type::getInt8Ty(Context), 0);
    }

    else if (strchr(parsed_type, '[') != nullptr && strchr(parsed_type, ']') != nullptr) {
        size_t left_bracket = strchr(parsed_type, '[') - parsed_type;
        char* base_type = strndup(parsed_type, left_bracket);
        llvm::Type* base_llvm_type = generate_type(parser, Context, base_type);
        free(base_type);

        size_t array_size = 0;
        const char* size_start = strchr(parsed_type, '[') + 1;
        const char* size_end = strchr(parsed_type, ']');
        if (size_start && size_end && size_end > size_start) {
            array_size = atoi(std::string(size_start, size_end - size_start).c_str());
        }

        if (array_size > 0) {
            return llvm::ArrayType::get(base_llvm_type, array_size);
        }

        return llvm::PointerType::get(base_llvm_type, 0);
    }

    throw std::runtime_error("Tipo não reconhecido: " + std::string(parsed_type));
}