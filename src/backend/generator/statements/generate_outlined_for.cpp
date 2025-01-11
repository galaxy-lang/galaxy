#include "backend/generator/statements/generate_outlined_for.hpp"
#include "backend/generator/statements/generate_stmt.hpp"
#include "backend/generator/statements/generate_variable_declaration_stmt.hpp"
#include "backend/generator/expressions/generate_expr.hpp"
#include "backend/generator/utils/generate_stop.hpp"
#include "backend/generator/symbols/identifier_symbol_table.hpp"


llvm::Function* generate_outlined_for(ForNode *node, llvm::LLVMContext &Context, llvm::Module &Module, llvm::GlobalVariable *ompIdent, char *schedule_policy) {
    // Criando a assinatura da função for.omp_outlined
    llvm::FunctionType* funcType = llvm::FunctionType::get(llvm::Type::getVoidTy(Context), 
        { llvm::PointerType::get(llvm::Type::getInt8Ty(Context), 0) }, false);

    llvm::Function* outlinedFunc = llvm::Function::Create(funcType, llvm::Function::PrivateLinkage, "for.omp_outlined", &Module);

    llvm::BasicBlock *entry = llvm::BasicBlock::Create(Context, "entry", outlinedFunc);
    llvm::IRBuilder<llvm::NoFolder> Builder(Context);
    Builder.SetInsertPoint(entry);

    // Declarar e inicializar a variável do loop
    llvm::Value *startVal = generate_expr(node->start, Context, Builder, Module);

    llvm::AllocaInst *loopVar = Builder.CreateAlloca(startVal->getType(), nullptr, node->variable);
    Builder.CreateStore(startVal, loopVar);  // Inicializar a variável do loop

    llvm::Value *loopVarVal = Builder.CreateLoad(loopVar->getAllocatedType(), loopVar, node->variable);
    llvm::Value *stopVal = generate_stop(node->stop, Context, Builder, Module);

    // Certifique-se de que ambos são do tipo i32
    if (loopVarVal->getType() != llvm::Type::getInt32Ty(Context)) {
        loopVarVal = Builder.CreateSExt(loopVarVal, llvm::Type::getInt32Ty(Context));
    }

    if (stopVal->getType() != llvm::Type::getInt32Ty(Context)) {
        stopVal = Builder.CreateSExt(stopVal, llvm::Type::getInt32Ty(Context));
    }

    // Definir as variáveis OpenMP para o loop
    llvm::AllocaInst *ompLb = Builder.CreateAlloca(llvm::Type::getInt32Ty(Context), nullptr, "omp.lb");
    llvm::AllocaInst *ompUb = Builder.CreateAlloca(llvm::Type::getInt32Ty(Context), nullptr, "omp.ub");
    llvm::AllocaInst *ompStride = Builder.CreateAlloca(llvm::Type::getInt32Ty(Context), nullptr, "omp.stride");
    llvm::AllocaInst *ompIsLast = Builder.CreateAlloca(llvm::Type::getInt32Ty(Context), nullptr, "omp.is_last");

    // Inicializar variáveis OpenMP
    Builder.CreateStore(loopVarVal, ompLb);  // Limite inferior
    Builder.CreateStore(stopVal, ompUb);    // Limite superior
    Builder.CreateStore(llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 1), ompStride); // Incremento
    Builder.CreateStore(llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 0), ompIsLast); // Última iteração (inicialmente 0)

    // Determinar o tipo de agendamento com base na política
    llvm::Value *scheduleType = nullptr;
    if (strcmp(schedule_policy, "static") == 0) {
        scheduleType = llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 33);
    } else if (strcmp(schedule_policy, "dynamic") == 0) {
        scheduleType = llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 35);
    } else if (strcmp(schedule_policy, "guided") == 0) {
        scheduleType = llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 36);
    } else {
        throw std::invalid_argument("Unsupported schedule policy");
    }

    // Parâmetros específicos para dynamic e guided
    llvm::Value *gtid = llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 0);
    llvm::Value *chunkSize = llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 4); // Tamanho do chunk

    // Inserir chamada para OpenMP
    if (strcmp(schedule_policy, "static") == 0) {
        // Para agendamento estático, usar __kmpc_for_static_init_4
        llvm::FunctionCallee kmpcForStaticInit = Module.getOrInsertFunction(
          "__kmpc_for_static_init_4",
          llvm::FunctionType::get(
              llvm::Type::getVoidTy(Context),
              {
                  llvm::PointerType::get(llvm::Type::getInt8Ty(Context), 0), // ident
                  llvm::Type::getInt32Ty(Context), // gtid
                  llvm::Type::getInt32Ty(Context), // schedule_type
                  llvm::PointerType::get(llvm::Type::getInt32Ty(Context), 0), // is_last
                  llvm::PointerType::get(llvm::Type::getInt32Ty(Context), 0), // lb
                  llvm::PointerType::get(llvm::Type::getInt32Ty(Context), 0), // ub
                  llvm::PointerType::get(llvm::Type::getInt32Ty(Context), 0), // stride
                  llvm::Type::getInt32Ty(Context), // chunk
                  llvm::Type::getInt32Ty(Context)  // increment
              },
              false
          )
      );

      Builder.CreateCall(kmpcForStaticInit, {
          ompIdent, 
          gtid, 
          scheduleType, 
          ompIsLast, 
          ompLb, 
          ompUb, 
          ompStride, 
          chunkSize, // Usado para dynamic/guided
          llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 1) // Incremento
      });
    } else if (strcmp(schedule_policy, "dynamic") == 0 || strcmp(schedule_policy, "guided") == 0) {
        // Para dynamic/guided, usar __kmpc_dispatch_init_4 e __kmpc_dispatch_next_4
        llvm::FunctionCallee kmpcDispatchInit = Module.getOrInsertFunction(
            "__kmpc_dispatch_init_4",
            llvm::FunctionType::get(
                llvm::Type::getVoidTy(Context),
                {
                    llvm::PointerType::get(llvm::Type::getInt8Ty(Context), 0), // ident
                    llvm::Type::getInt32Ty(Context), // gtid
                    llvm::Type::getInt32Ty(Context), // schedule_type
                    llvm::Type::getInt32Ty(Context), // lb
                    llvm::Type::getInt32Ty(Context), // ub
                    llvm::Type::getInt32Ty(Context), // stride
                    llvm::Type::getInt32Ty(Context)  // chunk
                },
                false
            )
        );

        llvm::Value *scheduleType = strcmp(schedule_policy, "dynamic") == 0
            ? llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 35)
            : llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 36);

        Builder.CreateCall(kmpcDispatchInit, {
            ompIdent,
            gtid,
            scheduleType,
            llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 0), // lb
            llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 100), // ub (exemplo)
            llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 1), // stride
            chunkSize
        });
    }

    llvm::BasicBlock *preLoopBB = llvm::BasicBlock::Create(Context, "preloop", outlinedFunc);
    llvm::BasicBlock *condBB = llvm::BasicBlock::Create(Context, "cond", outlinedFunc);
    llvm::BasicBlock *bodyBB = llvm::BasicBlock::Create(Context, "body", outlinedFunc);
    llvm::BasicBlock *updateBB = llvm::BasicBlock::Create(Context, "update", outlinedFunc);
    llvm::BasicBlock *endBB = llvm::BasicBlock::Create(Context, "endloop", outlinedFunc);

    Builder.CreateBr(preLoopBB);
    Builder.SetInsertPoint(preLoopBB);

    Builder.CreateBr(condBB);

    Builder.SetInsertPoint(condBB);
    llvm::Value *cond = Builder.CreateICmpSLT(loopVarVal, stopVal, "loopcond");

    Builder.CreateCondBr(cond, bodyBB, endBB);

    Builder.SetInsertPoint(bodyBB);

    for (size_t i = 0; i < node->body_count; i++) {
        generate_stmt(node->body[i], Context, Module, Builder);
    }

    Builder.CreateBr(updateBB);  // Jump to the update block

    // Update loop variable
    Builder.SetInsertPoint(updateBB);
    llvm::Value *inc = Builder.CreateAdd(loopVarVal, llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 1), "inc");
    Builder.CreateStore(inc, loopVar);  // Update %i

    Builder.CreateBr(condBB);  // Jump back to the condition block

    // Finalize the loop
    Builder.SetInsertPoint(endBB);
    Builder.CreateRetVoid();

    return outlinedFunc;
}
