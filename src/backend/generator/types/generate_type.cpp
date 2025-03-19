#include "backend/generator/types/generate_type.hpp"

llvm::Type* generate_type(const char* parsed_type, bool isPtr, llvm::LLVMContext &Context) {
    llvm::Type* base_type = nullptr;

    // Handle simple types
    if (strcmp(parsed_type, "i8") == 0) {
        base_type = llvm::Type::getInt8Ty(Context);  // Return 8-bit integer type
    } else if (strcmp(parsed_type, "i16") == 0) {
        base_type = llvm::Type::getInt16Ty(Context); // Return 16-bit integer type
    } else if (strcmp(parsed_type, "i32") == 0) {
        base_type = llvm::Type::getInt32Ty(Context); // Return 32-bit integer type
    } else if (strcmp(parsed_type, "i64") == 0) {
        base_type = llvm::Type::getInt64Ty(Context); // Return 64-bit integer type
    } else if (strcmp(parsed_type, "i128") == 0) {
        base_type = llvm::Type::getIntNTy(Context, 128); // Return 128-bit integer type
    } else if (strcmp(parsed_type, "u8") == 0) {
        base_type = llvm::Type::getInt8Ty(Context);  // Return 8-bit unsigned integer type
    } else if (strcmp(parsed_type, "u16") == 0) {
        base_type = llvm::Type::getInt16Ty(Context); // Return 16-bit unsigned integer type
    } else if (strcmp(parsed_type, "u32") == 0) {
        base_type = llvm::Type::getInt32Ty(Context); // Return 32-bit unsigned integer type
    } else if (strcmp(parsed_type, "u64") == 0) {
        base_type = llvm::Type::getInt64Ty(Context); // Return 64-bit unsigned integer type
    } else if (strcmp(parsed_type, "u128") == 0) {
        base_type = llvm::Type::getIntNTy(Context, 128); // Return 128-bit unsigned integer type
    } else if (strcmp(parsed_type, "isize") == 0) {
        base_type = llvm::Type::getInt64Ty(Context); // Assuming 64-bit architecture for isize
    } else if (strcmp(parsed_type, "usize") == 0) {
        base_type = llvm::Type::getInt64Ty(Context); // Assuming 64-bit architecture for usize
    } else if (strcmp(parsed_type, "f32") == 0) {
        std::vector<llvm::Type*> structFields;
        structFields.push_back(llvm::Type::getFloatTy(Context));           // Campo 0: float (4 bytes)
        structFields.push_back(llvm::ArrayType::get(llvm::Type::getInt8Ty(Context), 4)); // Campo 1: 4 bytes de preenchimento
        base_type = llvm::StructType::create(Context, structFields, "float_with_padding");
        // base_type = llvm::Type::getFloatTy(Context);  // Return 32-bit floating-point type
    } else if (strcmp(parsed_type, "f64") == 0) {
        base_type = llvm::Type::getDoubleTy(Context); // Return 64-bit floating-point type
    } else if (strcmp(parsed_type, "bool") == 0) {
        base_type = llvm::Type::getInt1Ty(Context);   // Return 1-bit boolean type
    } else if (strcmp(parsed_type, "void") == 0) {
        base_type = llvm::Type::getVoidTy(Context);   // Return void type (used for function returns)
    } else if (strcmp(parsed_type, "null") == 0) {
        base_type = llvm::Type::getInt8Ty(Context); // Return pointer type for null
    } else if (strcmp(parsed_type, "undefined") == 0) {
        base_type = llvm::Type::getInt8Ty(Context); // Return pointer type for undefined
    } else if (strcmp(parsed_type, "string") == 0) {
        // Handle string type as a pointer to a character array (int8_t)
        base_type = llvm::PointerType::get(llvm::Type::getInt8Ty(Context), 0); // Pointer to 8-bit integer (char)
    }

    // Handle composite types like arrays and slices
    else if (strncmp(parsed_type, "array", 5) == 0 || strncmp(parsed_type, "slice", 5) == 0) {
        // Check if the type has a parameterized inner type (e.g., array<int>, slice<float>)
        if (parsed_type[strlen(parsed_type) - 1] == '>') {
            // Find the type inside the angle brackets
            size_t start_pos = strchr(parsed_type, '<') - parsed_type + 1;
            size_t len = strlen(parsed_type) - start_pos - 1;
            char* inner_type = strndup(parsed_type + start_pos, len);  // Extract the inner type name
            
            // Recursively generate the LLVM type for the inner type
            llvm::Type* element_type = generate_type(inner_type, false, Context);
            free(inner_type);  // Clean up the allocated string for inner type
            
            // Return a pointer to the element type for the array or slice
            base_type = llvm::PointerType::get(element_type, 0);
        } else {
            // Return a pointer to int8_t for unsupported or unspecified array/slice types
            base_type = llvm::PointerType::get(llvm::Type::getInt8Ty(Context), 0);
        }
    }

    // Handle array types, e.g., int[10], float[5], etc.
    else if (strchr(parsed_type, '[') != nullptr && strchr(parsed_type, ']') != nullptr) {
        // Extract the base type of the array (everything before the '[')
        size_t left_bracket = strchr(parsed_type, '[') - parsed_type;
        char* base_type_str = strndup(parsed_type, left_bracket);
        llvm::Type* base_llvm_type = generate_type(base_type_str, false, Context);
        free(base_type_str);  // Clean up the allocated string for base type

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
            base_type = llvm::ArrayType::get(base_llvm_type, array_size);
        } else {
            // If no size is specified, return a pointer type (dynamically sized array)
            base_type = llvm::PointerType::get(base_llvm_type, 0);
        }
    }

    // If the type is not recognized, return a pointer to int8_t as a fallback
    if (!base_type) {
        base_type = llvm::PointerType::get(llvm::Type::getInt8Ty(Context), 0);
    }

    // Handle const and pointer qualifiers
    if (isPtr) {
        base_type = llvm::PointerType::get(base_type, 0);
    }

    return base_type;
}