#ifndef QUEUE_H
#define QUEUE_H

extern "C" {
  #include "frontend/ast/definitions.h"
}

#include <future>
#include <mutex>
#include <queue>
#include <condition_variable>

struct PendingIdentifier {
    IdentifierNode *node;
    std::promise<llvm::Value*> promise;
};

// Tabela de símbolos e fila de pendências
extern std::mutex symbolTableMutex;
extern std::queue<PendingIdentifier> pendingQueue;
extern std::condition_variable pendingCondition;

llvm::Value* find_or_wait_for_identifier(IdentifierNode *node);

void process_pending_identifiers_async();
void process_pending_identifiers_periodically();

#endif // QUEUE_H