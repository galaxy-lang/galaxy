#include "backend/generator/expressions/generate_expr.hpp"
#include "backend/generator/statements/generate_stmt.hpp"
#include "backend/generator/statements/generate_for_stmt.hpp"
#include "backend/generator/statements/generate_variable_declaration_stmt.hpp"
#include "backend/generator/symbols/identifier_symbol_table.hpp"

llvm::Value* generate_for_stmt(ForNode *node, llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder, llvm::Module &Module) {
    llvm::Function *currentFunction = Builder.GetInsertBlock()->getParent();

    // Blocos básicos do loop
    llvm::BasicBlock *preLoopBB = llvm::BasicBlock::Create(Context, "preloop", currentFunction);
    llvm::BasicBlock *condBB = llvm::BasicBlock::Create(Context, "cond", currentFunction);
    llvm::BasicBlock *bodyBB = llvm::BasicBlock::Create(Context, "body", currentFunction);
    llvm::BasicBlock *updateBB = llvm::BasicBlock::Create(Context, "update", currentFunction);
    llvm::BasicBlock *endBB = llvm::BasicBlock::Create(Context, "endloop", currentFunction);

    // Cria o salto para o bloco de pré-loop
    Builder.CreateBr(preLoopBB);
    Builder.SetInsertPoint(preLoopBB);

    // Declaração da variável do loop (%i) e inicialização
    VariableNode iteratorVar;
    iteratorVar.name = node->variable;
    iteratorVar.varType = node->var_type;
    iteratorVar.isPtr = node->var_isPtr;
    iteratorVar.isConst = false;
    iteratorVar.value = node->start;

    generate_variable_declaration_stmt(&iteratorVar, Context, Builder, Module);

    llvm::Value *startVal = generate_expr(node->start, Context, Builder, Module);

    llvm::AllocaInst *loopVar = Builder.CreateAlloca(startVal->getType(), nullptr, node->variable);
    Builder.CreateStore(startVal, loopVar);  // Inicializa %i

    Builder.CreateBr(condBB);

    // Condição do loop
    Builder.SetInsertPoint(condBB);
    llvm::Value *loopVarVal = Builder.CreateLoad(loopVar->getAllocatedType(), loopVar, node->variable);

    BinaryExprNode *binaryNode = (BinaryExprNode *)node->stop->data;
    llvm::Value *rightOperand = generate_expr(binaryNode->right, Context, Builder, Module);
    llvm::Value *stopVal = rightOperand;

    if (stopVal->getType()->isPointerTy()) {
        const SymbolInfo *id = find_identifier(stopVal->getName().str());
        stopVal = id->value;
    }

    llvm::Value *cond = Builder.CreateICmpSLT(loopVarVal, stopVal, "loopcond");

    Builder.CreateCondBr(cond, bodyBB, endBB);  // Se a condição for verdadeira, vai para o corpo; senão, para o final.

    // Corpo do loop
    Builder.SetInsertPoint(bodyBB);
    for (size_t i = 0; i < node->body_count; ++i) {
        generate_stmt(node->body[i], Context, Module, Builder);  // Gerar as declarações no corpo do loop
    }

    Builder.CreateBr(updateBB);  // Salta para o bloco de atualização

    // Atualiza o valor de %i
    Builder.SetInsertPoint(updateBB);
    llvm::Value *loopVarValUpdate = Builder.CreateLoad(loopVar->getAllocatedType(), loopVar, node->variable);
    llvm::Value *inc = Builder.CreateAdd(loopVarValUpdate, llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 1), "inc");
    Builder.CreateStore(inc, loopVar);  // Atualiza %i

    Builder.CreateBr(condBB);  // Salta de volta para a condição

    // Finaliza o loop
    Builder.SetInsertPoint(endBB);
    return Builder.CreateLoad(loopVar->getAllocatedType(), loopVar, node->variable);
}
