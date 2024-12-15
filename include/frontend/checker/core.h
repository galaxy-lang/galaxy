#ifndef CHECKER_CORE_H
#define CHECKER_CORE_H
#include <map>
#include "../ast/definitions.h"

struct TypeDef {
	char* name;
	int id;
	Typedef** entries;
}

struct Namespace {
	map<char*, TypeDef>* space;
	int varcountn
}
struct Checker {
	AstNode* ast;
	Namespace** scopes;
	Namespace* scope;
	Namespace* typedefs;
}

//TODO: add methods
#endif //CHECKER_CORE_H
