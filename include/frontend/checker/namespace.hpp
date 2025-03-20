#ifndef NAMESPACE_HPP
#define NAMESPACE_HPP
#include "frontend/checker/typedef.hpp"
#include "frontend/checker/result.hpp"
#include <string>

template <typename T>
using resultstr = gutil::Result<T, std::string>;
class Namespace {
	private:
		gtable<Typedef*> names;
		gtable<bool> consts;
		Namespace* parent;

	public:
		Namespace();
		Namespace(Namespace* parent);

		resultstr<Typedef*> get(std::string k);
		// only searches on local namespace
		resultstr<Typedef*> strictget(std::string k);
		resultstr<bool> set(std::string k, Typedef* tyv);
		resultstr<bool> setvar(std::string k, Typedef* tyv);
		resultstr<bool> change(std::stringk, Typedef* tyv);

		bool has(std::string k);
}
#endif //NAMESPACE_HPP
