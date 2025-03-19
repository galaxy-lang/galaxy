#ifndef TYPEDEF_HPP
#define TYPEDEF_HPP
#include <unordered_map>
#include <string>
#include <vector>
#include "frontend/checker/dict.hpp"
#include "frontend/checker/result.hpp"

using namespace gutil;
template <typename T>
using gtable = std::unordered_map<std::string, T>

using gdict = Dictionary;
struct Typedef {
	std::string name;
	std::vector<Typedef*> entries; // reserved
	int id;
	gtable<bool> flags;
	gdict attrs;
	Typedef(std::string name, int id, gdict attrs);

	static Typedef* generic();
}

class Typespace {
	gtable<Typedef*> types;
	gtable<Typedef*> subst;

	void insert(Typedef* ty);
	Typedef* get(std::string name);

	Typedef* operator[](std::string name)>
	Typedef* unify(Typedef* t1, Typedef* t2);

	~Typespace();
}
#endif //TYPEDEF_HPP
