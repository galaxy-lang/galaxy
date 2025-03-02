#include "backend/generator/expressions/generate_array_access.hpp"
#include "backend/generator/expressions/generate_expr.hpp"
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>

llvm::Value *generate_array_access(ArrayAccessNode *node, llvm::LLVMContext &Context, llvm::IRBuilder<llvm::NoFolder> &Builder, llvm::Module &Module) {
    // 1. Obter o array e o índice do nó
    AstNode *arrayNode = node->array;
    AstNode *indexNode = node->index;

    if (!arrayNode || !indexNode) {
        llvm::errs() << "Erro: Array ou índice nulo no nó de acesso a array.\n";
        return nullptr;
    }

    // 2. Gerar o código LLVM para o array e o índice
    llvm::Value *arrayValue = generate_expr(arrayNode, Context, Builder, Module);
    llvm::Value *indexValue = generate_expr(indexNode, Context, Builder, Module);

    if (!arrayValue || !indexValue) {
        llvm::errs() << "Erro: Falha ao gerar código para array ou índice.\n";
        return nullptr;
    }

    // 3. Verificar se o arrayValue é um ponteiro para um array
    if (!arrayValue->getType()->isPointerTy()) {
        llvm::errs() << "Erro: O array não é um ponteiro.\n";
        return nullptr;
    }

    // 4. Verificar se o índice é um valor inteiro
    if (!indexValue->getType()->isIntegerTy()) {
        // Converter o índice para um tipo inteiro (i32) se não for inteiro
        indexValue = Builder.CreateIntCast(indexValue, llvm::Type::getInt32Ty(Context), true, "idxcast");
    }

    // 5. Criar a instrução GEP (GetElementPtr) para acessar o elemento do array
    llvm::Type *elementType = arrayValue->getType()->getArrayElementType(); // Tipo do elemento apontado
    llvm::Value *indices[] = {
        Builder.getInt32(0), // Primeiro índice: 0 para acessar o primeiro elemento do array
        indexValue           // Segundo índice: o índice fornecido
    };

    llvm::Value *elementPtr = Builder.CreateGEP(
        elementType, // Tipo do elemento apontado
        arrayValue,  // Ponteiro base
        indices,     // Índices
        "arrayidx"   // Nome do valor resultante
    );

    if (!elementPtr) {
        llvm::errs() << "Erro: Falha ao criar instrução GEP.\n";
        return nullptr;
    }

    // 6. Carregar o valor do elemento do array
    llvm::Value *elementValue = Builder.CreateLoad(
        elementType, // Tipo do valor a ser carregado
        elementPtr,  // Ponteiro
        "arrayelem"  // Nome do valor carregado
    );

    return elementValue;
}