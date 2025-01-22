#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "frontend/ast/definitions.h"
#include "frontend/checker/core.hpp"
#include "frontend/checker/common_types.hpp"

Checker::Checker(AstNode* ast) {
	this->ast = ast;

	push_scope();
}

void Checker::push_scope() {
	Namespace* ns;
	if (scope) {
		ns = new Namespace(scope);
	} else {
		ns = new Namespace(nullptr);
	}

	if (ns == nullptr) {
		std::cerr << "Error on allocating a new namespace" << std::endl;
		exit(1);
	}

	scopes.push_back(ns);
	scope = ns;
}

void Checker::pop_scope() {
	if (scopes.empty()) {
		std::cerr << "CONGRULATIONS!!!!!!!!!!!!!!!!!!! IM REALLY PROUD OF YOU, my dear galactic programmer :3 are you trying to pop scope nothing???? wooow you're fucking smart :DDDD i wanna be like you when I grow up" << std::endl; //acho que o usuario da linguagem n vai ver isso
                exit(1);
	}
	delete scopes[scopes.size() - 1];
	scopes.pop_back();
	scope = scopes[scopes.size() - 1];
}

void Checker::insert_ty(Typedef* ty) {
	typedefs[ty->name] = ty;
}

Typedef* Checker::get_ty(std::string name) {
	return typedefs.at(name);
}

Checker::~Checker() {
	for (auto e : scopes) {
		delete e;
	}
	scope = nullptr;
}
