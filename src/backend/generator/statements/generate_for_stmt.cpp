#include "backend/generator/utils/generate_stop.hpp"
#include "backend/generator/expressions/generate_expr.hpp"
#include "backend/generator/statements/generate_stmt.hpp"
#include "backend/generator/statements/generate_for_stmt.hpp"
#include "backend/generator/statements/generate_variable_declaration_stmt.hpp"
#include "backend/generator/statements/generate_outlined_for.hpp"
#include "backend/generator/symbols/identifier_symbol_table.hpp"
#include "backend/generator/symbols/iterator_stack.hpp"
#include "backend/generator/types/generate_type.hpp"

llvm::Value* generate_for_stmt(ForNode *node, llvm::LLVMContext &Context, llvm::IRBuilder<llvm::NoFolder> &Builder, llvm::Module &Module) {
    llvm::Function *currentFunction = Builder.GetInsertBlock()->getParent();
    
    VariableNode iteratorVar;
    iteratorVar.name = node->variable;
    iteratorVar.varType = node->var_type;
    iteratorVar.isPtr = node->var_isPtr;
    iteratorVar.isConst = false;
    iteratorVar.value = node->start;


    llvm::Value *decl;
    llvm::Value *iterator;

    if (node->iterator) {
        iterator = generate_expr(node->iterator, Context, Builder, Module);
        decl = generate_variable_declaration_stmt(&iteratorVar, Context, Builder, Module);
    }
    
    llvm::Value *startVal = generate_expr(node->start, Context, Builder, Module);

    push_iterator(decl, iterator, iterator->getType());

    llvm::AllocaInst *loopVar = Builder.CreateAlloca(startVal->getType(), nullptr, node->variable);
    Builder.CreateStore(startVal, loopVar);  // Inicializar a variável do loop

    {
        std::lock_guard<std::mutex> lock(symbol_stack_mutex);
        add_identifier(node->variable, loopVar, startVal, startVal->getType());
    }

    llvm::Value *stopVal = generate_stop(node->stop, Context, Builder, Module);

    if (startVal->getType() != llvm::Type::getInt32Ty(Context)) {
        startVal = Builder.CreateSExt(startVal, llvm::Type::getInt32Ty(Context));
    }

    if (stopVal->getType() != llvm::Type::getInt32Ty(Context)) {
        stopVal = Builder.CreateLoad(llvm::Type::getInt32Ty(Context), stopVal, "stopval");
        stopVal = Builder.CreateSExt(stopVal, llvm::Type::getInt32Ty(Context));
    }

    if (node->is_parallel) {
        llvm::Constant *ompString = llvm::ConstantDataArray::getString(Context, ";unknown;unknown;0;0;;", true);

        llvm::GlobalVariable *ompGlobalString = new llvm::GlobalVariable(
            Module,
            ompString->getType(),
            true,
            llvm::GlobalValue::PrivateLinkage,
            ompString,
            "omp_global_string"
        );

        llvm::StructType *identTy = llvm::StructType::get(
            Context,
            {
                llvm::Type::getInt32Ty(Context),                         // Reserved
                llvm::Type::getInt32Ty(Context),                         // Flags
                llvm::Type::getInt32Ty(Context),                         // Reserved
                llvm::Type::getInt32Ty(Context),                         // Source Info
                llvm::PointerType::get(llvm::Type::getInt8Ty(Context), 0) // String Pointer
            }
        );

        auto create_omp_ident = [&](int flags, llvm::StringRef name) {
            llvm::Constant *ident = llvm::ConstantStruct::get(
                identTy,
                {
                    llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 0),
                    llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), flags),
                    llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 0),
                    llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 22),
                    llvm::ConstantExpr::getBitCast(ompGlobalString, llvm::PointerType::get(llvm::Type::getInt8Ty(Context), 0))
                }
            );
            return new llvm::GlobalVariable(
                Module,
                identTy,
                true,
                llvm::GlobalValue::PrivateLinkage,
                ident,
                name
            );
        };

        llvm::FunctionCallee kmpcGlobalThreadNum = Module.getOrInsertFunction(
            "__kmpc_global_thread_num",
            llvm::FunctionType::get(
                llvm::Type::getInt32Ty(Context),
                { llvm::PointerType::get(identTy, 0) },
                false
            )
        );

        llvm::FunctionCallee kmpcPushNumThreads = Module.getOrInsertFunction(
            "__kmpc_push_num_threads",
            llvm::FunctionType::get(
                llvm::Type::getVoidTy(Context),
                {
                    llvm::PointerType::get(identTy, 0),
                    llvm::Type::getInt32Ty(Context),
                    llvm::Type::getInt32Ty(Context)
                },
                false
            )
        );

        llvm::FunctionCallee kmpcForkCall = Module.getOrInsertFunction(
            "__kmpc_fork_call",
            llvm::FunctionType::get(
                llvm::Type::getVoidTy(Context),
                {
                    llvm::PointerType::get(identTy, 0),
                    llvm::Type::getInt32Ty(Context),
                    llvm::PointerType::get(llvm::Type::getVoidTy(Context), 0)
                },
                true
            )
        );

        llvm::GlobalVariable *ompIdent0 = create_omp_ident(514, "omp_ident0");
        llvm::GlobalVariable *ompIdent1 = create_omp_ident(2, "omp_ident1");

        llvm::GlobalVariable *ompIdent = create_omp_ident(514, "omp_ident");

        llvm::Value *threadNum = Builder.CreateCall(kmpcGlobalThreadNum, { ompIdent1 });

        Builder.CreateCall(kmpcPushNumThreads, { ompIdent1, threadNum, llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 1) });

        llvm::Function* ompOutlined = generate_outlined_for(node, Context, Module, ompIdent, node->schedule_policy);

        Builder.CreateCall(kmpcForkCall, { ompIdent1, llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), 0), ompOutlined });
    } else {
        llvm::BasicBlock *preLoopBB = llvm::BasicBlock::Create(Context, "preloop", currentFunction);
        llvm::BasicBlock *condBB = llvm::BasicBlock::Create(Context, "cond", currentFunction);
        llvm::BasicBlock *bodyBB = llvm::BasicBlock::Create(Context, "body", currentFunction);
        llvm::BasicBlock *updateBB = llvm::BasicBlock::Create(Context, "update", currentFunction);
        llvm::BasicBlock *endBB = llvm::BasicBlock::Create(Context, "endloop", currentFunction);

        Builder.CreateBr(preLoopBB);
        Builder.SetInsertPoint(preLoopBB);

        Builder.CreateBr(condBB);

        Builder.SetInsertPoint(condBB);
        llvm::Value *loopVarVal = Builder.CreateLoad(loopVar->getAllocatedType(), loopVar, node->variable);
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
        Builder.CreateStore(inc, loopVar);  // Update

        Builder.CreateBr(condBB);  // Jump back to the condition block

        // Finalize the loop
        Builder.SetInsertPoint(endBB);
    }

    pop_iterator();

    return nullptr;
}
