#include "backend/generator/types/generate_type.hpp"

llvm::Type* generate_type(Parser *parser, llvm::LLVMContext &Context, const char* parsed_type) {
    // Handle simple types
    if (strcmp(parsed_type, "int") == 0) {
        return llvm::Type::getInt32Ty(Context);  // Return 32-bit integer type
    } else if (strcmp(parsed_type, "float") == 0) {
        return llvm::Type::getFloatTy(Context);  // Return 32-bit floating-point type
    } else if (strcmp(parsed_type, "double") == 0) {
        return llvm::Type::getDoubleTy(Context); // Return 64-bit floating-point type
    } else if (strcmp(parsed_type, "bool") == 0) {
        return llvm::Type::getInt1Ty(Context);   // Return 1-bit boolean type
    } else if (strcmp(parsed_type, "void") == 0) {
        return llvm::Type::getVoidTy(Context);   // Return void type (used for function returns)
    } else if (strcmp(parsed_type, "string") == 0) {
        // Handle string type as a pointer to a character array (int8_t)
        return llvm::PointerType::get(llvm::Type::getInt8Ty(Context), 0); // Pointer to 8-bit integer (char)
    }

    // Handle composite types like lists or tuples
    else if (strncmp(parsed_type, "list", 4) == 0 || strncmp(parsed_type, "tuple", 5) == 0) {
        // Check if the type has a parameterized inner type (e.g., list<int>, tuple<float>)
        if (parsed_type[strlen(parsed_type) - 1] == '>') {
            // Find the type inside the angle brackets
            size_t start_pos = strchr(parsed_type, '<') - parsed_type + 1;
            size_t len = strlen(parsed_type) - start_pos - 1;
            char* inner_type = strndup(parsed_type + start_pos, len);  // Extract the inner type name
            
            // Recursively generate the LLVM type for the inner type
            llvm::Type* element_type = generate_type(parser, Context, inner_type);
            free(inner_type);  // Clean up the allocated string for inner type
            
            // Return a pointer to the element type for the list or tuple
            return llvm::PointerType::get(element_type, 0);
        }

        // Return a pointer to int8_t for unsupported or unspecified list/tuple types
        return llvm::PointerType::get(llvm::Type::getInt8Ty(Context), 0);
    }

    // Handle array types, e.g., int[10], float[5], etc.
    else if (strchr(parsed_type, '[') != nullptr && strchr(parsed_type, ']') != nullptr) {
        // Extract the base type of the array (everything before the '[')
        size_t left_bracket = strchr(parsed_type, '[') - parsed_type;
        char* base_type = strndup(parsed_type, left_bracket);
        llvm::Type* base_llvm_type = generate_type(parser, Context, base_type);
        free(base_type);  // Clean up the allocated string for base type

        // Parse the array size from the brackets (e.g., 10 in int[10])
        size_t array_size = 0;
        const char* size_start = strchr(parsed_type, '[') + 1;
        const char* size_end = strchr(parsed_type, ']');
        if (size_start && size_end && size_end > size_start) {
            // Convert the size from string to integer
            array_size = atoi(std::string(size_start, size_end - size_start).c_str());
        }

        // If array size is specified, create an array type of that size
        if (array_size > 0) {
            return llvm::ArrayType::get(base_llvm_type, array_size);
        }

        // If no size is specified, return a pointer type (dynamically sized array)
        return llvm::PointerType::get(base_llvm_type, 0);
    }
}
