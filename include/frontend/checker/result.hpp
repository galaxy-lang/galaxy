#ifndef RESULT_HPP
#define RESULT_HPP

namespace gutil {

template <typename T, typename E>
class Result {
        private:
                bool iserror;
		T value;
		E err;

		Result(bool errb, T val) : iserror(errb), value(val) { };
		Result(bool errb, E error) : iserror(errb), err(error) { };
        public:
		static Result<T, E> okay(T e) {
			return Result(false, e);
		};

                static Result<T, E> error(E e) {
			return Result(true, e);
		};

				
		bool isokay() {
			return !iserror;
		};
		bool iserr() {
			return iserror;
		};

		template <typename Y>
		Y unwrap() {
			if (iserror) {
				return (Y)err;
			};
			return (Y)value;
		}
};
}
#endif //RESULT_HPP
