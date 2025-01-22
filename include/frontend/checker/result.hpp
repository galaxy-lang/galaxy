#ifndef RESULT_HPP
#define RESULT_HPP
#include <variant>

template <typename T, typename E>
class Result {
        private:
                bool iserror;
		std::variant<T, E> value;        

		Result(bool err, std::variant<T, E> _value) : iserror(err), value(_value) {};
        public:
		static Result<T, E> okay(T e) {
			return Result(false, e);
		}

                static Result<T, E> error(E e) {
			return Result(true, e);
		}

				
		bool isokay() {
			return !iserror;
		}
		bool iserr() {
			return iserror;
		}
		
	        template <typename Y>
		Y unwrap() const {
			return std::get<Y>(value);
		}
}
#endif //RESULT_HPP
