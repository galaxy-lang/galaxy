#ifndef CHECKER_CORE_H
#define CHECKER_CORE_H
#include <map>
#include "frontend/ast/definitions.h"

struct Typedef {
	char* name;
	int id;
	Typedef** entries;
}

struct Namespace {
	map<char*, Typedef>* space;
	int varcountn;
}
struct Checker {
	AstNode* ast;
	Namespace** scopes;
	Namespace* scope;
	Namespace* typedefs;
}

Checker* init(AstNode* ast);
void push_scope()
void push_variable(Checker* checker, char* name, Typedef* type);
void push_type(Checker* checker, char* tname, Typedef** templates);
TypeDef* check_node(Checker* checker, AstMode* node);
void freeChecker(Checker* checker);
#endif //CHECKER_CORE_H
