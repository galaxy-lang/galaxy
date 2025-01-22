#ifndef CHECKER_CORE_H
#define CHECKER_CORE_H
#include <unordered_map>
#include <vector>
#include <string>
#include "frontend/ast/definitions.h"
#include "./result.hpp"

template <typename T>
using GTable = std::unordered_map<std::string, T>;

using Typespace = GTable<Typedef*>;

struct Typedef {
	std::string name;
	int id;
	Typespace entries;
	GTable<bool> flags;
}

struct Namespace {
	Typespace& space;
	GTable<bool> consts;
	Namespace* parent;

	Result<Typedef*, std::string> get(std::string name);
	Result<bool, std::string> insert(bool isconst, std::string name, Typedef* ty);
	Result<bool, std::string> change(std::string name, Typedef* ty);
}



class Checker {
	private:
		AstNode* ast;
		std::vector<Namespace*> scopes;
		Namespace* scope;
		Typespace typedefs;

	public:
		Checker(AstNode* ast);

		void push_scope();
		void pop_scope();

		void insert_ty(Typedef* ty);
		Typedef* get_ty(std::string name);
		AstNode* check(AstNode* node);

		~Checker();
}

#endif //CHECKER_CORE_H
