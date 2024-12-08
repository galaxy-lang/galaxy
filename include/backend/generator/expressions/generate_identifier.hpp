#ifndef GENERATE_IDENTIFIER_H
#define GENERATE_IDENTIFIER_H

extern "C" {
  #include "frontend/ast/definitions.h"
}
#include <map>
#include <string>

#include "llvm/IR/Value.h"

extern std::map<std::string, llvm::Value *> NamedValues;

llvm::Value *generate_identifier(IdentifierNode *node);

#endif // GENERATE_IDENTIFIER_H