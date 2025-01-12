#include "backend/generator/statements/generate_if_stmt.hpp"
#include "backend/generator/statements/generate_stmt.hpp"
#include "backend/generator/expressions/generate_expr.hpp"
#include "backend/generator/parallel/queue.hpp"

llvm::Value* generate_if_stmt(IfNode *node, llvm::LLVMContext &Context, llvm::IRBuilder<llvm::NoFolder> &Builder, llvm::Module &Module) {
    
    global_id_return = "declaration";
    llvm::Value *condition = generate_expr(node->condition, Context, Builder, Module);
    if (!condition) {
        throw std::runtime_error("Failed to generate condition for if statement.");
    }

    // Criar os blocos de controle dentro da função atual
    llvm::BasicBlock *currentBB = Builder.GetInsertBlock();  // Obter o bloco atual da função
    llvm::Function *currentFunction = currentBB->getParent();  // Obter a função atual

    llvm::BasicBlock *thenBlock = llvm::BasicBlock::Create(Context, "then", currentFunction);
    llvm::BasicBlock *elseBlock = llvm::BasicBlock::Create(Context, "else", currentFunction);
    llvm::BasicBlock *mergeBlock = llvm::BasicBlock::Create(Context, "ifcont", currentFunction);

    // Instrução de desvio condicional
    Builder.CreateCondBr(condition, thenBlock, elseBlock);

    // Inserir as instruções no bloco 'then'
    Builder.SetInsertPoint(thenBlock);
    for (size_t i = 0; i < node->consequent_count; ++i) {
        generate_stmt(node->consequent[i], Context, Module, Builder);  // Gerar o código para cada instrução no 'then'
    }
    Builder.CreateBr(mergeBlock);  // Depois de executar o 'then', desvia para o bloco final

    // Inserir as instruções no bloco 'else' (caso exista)
    Builder.SetInsertPoint(elseBlock);
    if (node->alternate != nullptr) {
        for (size_t i = 0; i < node->alternate_count; ++i) {
            generate_stmt(node->alternate[i], Context, Module, Builder);  // Gerar o código para cada instrução no 'else'
        }
    }
    Builder.CreateBr(mergeBlock);  // Depois de executar o 'else', desvia para o bloco final

    // Inserir o código no bloco 'merge' (final)
    Builder.SetInsertPoint(mergeBlock);

    return nullptr;
}
