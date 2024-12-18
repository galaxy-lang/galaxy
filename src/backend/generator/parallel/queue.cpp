#include "backend/generator/expressions/generate_identifier.hpp"
#include "backend/generator/symbols/identifier_symbol_table.hpp"
#include "backend/generator/parallel/queue.hpp"

std::mutex symbolTableMutex;
std::queue<PendingIdentifier> pendingQueue;
std::condition_variable pendingCondition;

// Função para encontrar um identificador ou adicionar à fila de pendências
llvm::Value* find_or_wait_for_identifier(IdentifierNode* node) {
    std::unique_lock<std::mutex> lock(symbolTableMutex);

    // Tentar encontrar o identificador na tabela de símbolos
    const SymbolInfo* id = find_identifier(node->symbol);
    if (id) {
        return id->value;
    }

    // Se não encontrar, criar uma pendência
    PendingIdentifier pending{node};
    std::promise<llvm::Value*> promise;
    pending.promise = std::move(promise);
    auto future = pending.promise.get_future();

    // Adicionar à fila de pendências
    pendingQueue.push(std::move(pending));
    pendingCondition.notify_all();
    
    lock.unlock();  // Desbloquear a tabela de símbolos para permitir outros acessos

    // Esperar pela resolução do identificador
    return future.get();
}

// Processar as pendências de identificadores de forma assíncrona
void process_pending_identifiers_async() {
    std::unique_lock<std::mutex> lock(symbolTableMutex);

    // Processar a fila de pendências enquanto houver identificadores pendentes
    while (!pendingQueue.empty()) {
        PendingIdentifier pending = std::move(pendingQueue.front());
        pendingQueue.pop();

        // Verificar novamente a tabela de símbolos
        const SymbolInfo* id = find_identifier(pending.node->symbol);
        if (id) {
            // Resolver a pendência, setando o valor
            pending.promise.set_value(id->value);
        } else {
            // Se não encontrar, re-adiciona à fila
            pendingQueue.push(std::move(pending));
        }
    }
}

// Função para ser chamada em outro thread que processa as pendências
void process_pending_identifiers_periodically() {
    while (true) {
        process_pending_identifiers_async();
    }
}

