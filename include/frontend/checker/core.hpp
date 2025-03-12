#ifndef CHECKER_CORE_H
#define CHECKER_CORE_H
#include <unordered_map>
#include <vector>
#include <string>
#include "frontend/ast/definitions.h"
#include "frontend/checker/typedef.hpp"
#include "frontend/checker/result.hpp"
#include "frontend/checker/namespace.hpp"


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
		AstNode* analyze(AstNode* node);
		
		template <typename T>
		AstNode* check(T* nodedata);

		~Checker();
}

#endif //CHECKER_CORE_H
