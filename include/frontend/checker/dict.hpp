#ifndef DICT_HPP
#define DICT_HPP
#include <type_traits>
#include <functional>
#include <unordered_map>
#include <iostream>

namespace glxutil {
//to make generic value lists
struct GenericValue { 
	virtual ~GenericValue() {}
};

//to produce any kind of destroyable value
template <typename T, typename destrt>
struct Value : GenericValue {
	T value;
	destrt& destr;
	Value(T _v, destrt& ds) : value(_v), destr(ds) { }

	~Value() override {
		destr(value);
	}
};


template <typename T>
void _std_nonptr_destr(T& e) {};
class Dictionary {
	private:
		std::unordered_map<std::string, GenericValue*> symbols;

	public:
		template<typename T, typename destrt = std::function<void(T&)>>
		T& get(std::string k) {
			return (static_cast<Value<T, destrt>*>(symbols[k]))->value;
		};

		template <typename T, typename destrt = std::function<void(T&)>>
		void set(std::string k, T v, destrt& destructor) {

			symbols[k] = new Value<T, destrt>(v, destructor);
		};

		template <typename T>
		void set(std::string k, T v) {
			set<T>(k, v, _std_nonptr_destr<T>);
		};

		~Dictionary() {
			for (auto& kv : symbols) {
				delete kv.second;
			}
		};
};
}
#endif // DICT_HPP
