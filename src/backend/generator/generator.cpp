#include "backend/generator/generate_ir.hpp"
#include "backend/generator/statements/generate_stmt.hpp"
#include <future>
#include <mutex>

// Estrutura intermediária personalizada para armazenar informações de IR
struct IntermediateIR {
    std::string blockName;
    std::vector<llvm::Instruction*> instructions;
};

// Função principal de geração
std::vector<llvm::Value*> generate_ir(AstNode *node, llvm::LLVMContext &mainContext, llvm::Module &mainModule, llvm::IRBuilder<> &mainBuilder) {
    ProgramNode *program = static_cast<ProgramNode *>(node->data);
    std::vector<std::future<IntermediateIR>> futures;
    std::vector<IntermediateIR> intermediateIRs;

    // Geração paralela de IR
    for (size_t i = 0; i < program->statement_count; ++i) {
        AstNode *statement = program->statements[i];

        futures.push_back(std::async(std::launch::async, [statement]() -> IntermediateIR {
            // Criar estrutura intermediária para esta thread
            IntermediateIR ir;
            ir.blockName = "Block_" + std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id()));

            // Criar contexto, módulo e IRBuilder para esta thread
            llvm::LLVMContext threadContext;
            llvm::Module threadModule("ThreadModule", threadContext);
            llvm::IRBuilder<> threadBuilder(threadContext);

            llvm::Value* result = generate_stmt(statement, threadContext, threadModule, threadBuilder);
            if (auto *inst = llvm::dyn_cast<llvm::Instruction>(result)) {
                ir.instructions.push_back(inst); // Adicionar instrução gerada
            }

            return ir;
        }));
    }

    // Coletar resultados das threads
    for (auto &future : futures) {
        try {
            intermediateIRs.push_back(future.get());
        } catch (const std::exception &e) {
            llvm::errs() << "Exception during future.get(): " << e.what() << "\n";
        } catch (...) {
            llvm::errs() << "Unknown exception during future.get()\n";
        }
    }

    // Concatenar IRs intermediários no contexto principal
    for (const auto &ir : intermediateIRs) {
        // Criar um novo bloco no contexto principal

        /*
         * O segfault ocorre no getParent
         */

        llvm::BasicBlock *block = llvm::BasicBlock::Create(mainContext, ir.blockName, mainBuilder.GetInsertBlock()->getParent());
        mainBuilder.SetInsertPoint(block); // Estabelecer ponto de inserção no novo bloco

        // Inserir as instruções no bloco
        for (auto *inst : ir.instructions) {
            mainBuilder.Insert(inst); // Adicionar instrução ao bloco principal
        }
    }

    return {};
}
