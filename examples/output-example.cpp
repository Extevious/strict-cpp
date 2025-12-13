#if !defined(STRICT_CPP_OPTIONAL_TYPES)
	#define STRICT_CPP_OPTIONAL_TYPES
#endif

#include "strict_types/float.hpp"
#include "strict_types/integral.hpp"

#include <iostream>

void output(const auto&... out) { (std::cout << ... << out) << '\n'; }

void output(const auto& msg, const bool out) { std::cout << msg << (out ? "true" : "false") << '\n'; }

void w_output(const auto&... out) { (std::wcout << ... << out) << '\n'; }

void nl() { std::cout << '\n'; }

void arithmetic_operators(const auto lhs, const auto rhs) {
	output("Arithmetic operators:");
	output("lhs + rhs: ", lhs + rhs);
	output("lhs - rhs: ", lhs - rhs);
	output("lhs * rhs: ", lhs * rhs);
	output("lhs / rhs: ", lhs / rhs);
	output("lhs % rhs: ", lhs % rhs);

	auto _lhs = lhs;
	output("lhs += rhs: ", _lhs += rhs);
	_lhs = lhs;
	output("lhs -= rhs: ", _lhs -= rhs);
	_lhs = lhs;
	output("lhs *= rhs: ", _lhs *= rhs);
	_lhs = lhs;
	output("lhs /= rhs: ", _lhs /= rhs);
	_lhs = lhs;
	output("lhs %= rhs: ", _lhs %= rhs);
	nl();
}

void unary_operators(const auto lhs) {
	output("Unary operators:");
	output("+lhs: ", +lhs);
	output("-lhs: ", -lhs);
	nl();
}

void binary_operators(const auto lhs, const auto rhs) {
	output("Binary operators:");
	output("~lhs: ", ~lhs);
	output("lhs & rhs: ", lhs & rhs);
	output("lhs << rhs: ", lhs << rhs);
	output("lhs >> rhs: ", lhs >> rhs);
	output("lhs | rhs: ", lhs | rhs);
	output("lhs ^ rhs: ", lhs ^ rhs);

	auto _lhs = lhs;
	output("lhs &= rhs: ", _lhs &= rhs);
	_lhs = lhs;
	output("lhs <<= rhs: ", _lhs <<= rhs);
	_lhs = lhs;
	output("lhs >>= rhs: ", _lhs >>= rhs);
	_lhs = lhs;
	output("lhs |= rhs: ", _lhs |= rhs);
	_lhs = lhs;
	output("lhs ^= rhs: ", _lhs ^= rhs);
	nl();
}

void comparison_operators(const auto lhs, const auto rhs) {
	output("Comparison operators:");
	output("lhs == rhs: ", lhs == rhs);
	output("lhs != rhs: ", lhs != rhs);
	output("lhs > rhs: ", lhs > rhs);
	output("lhs < rhs: ", lhs < rhs);
	output("lhs >= rhs: ", lhs >= rhs);
	output("lhs <= rhs: ", lhs <= rhs);
	nl();
}

void incremental_operators(const auto lhs) {
	auto _lhs = lhs;

	output("Incremental operators:");
	output("++lhs: ", ++_lhs);
	output("++lhs: ", ++_lhs);
	output("++lhs: ", ++_lhs);
	output("lhs++: ", _lhs++);
	output("lhs++: ", _lhs++);
	output("lhs++: ", _lhs++);
	nl();
}

void decremental_operators(const auto lhs) {
	auto _lhs = lhs;

	output("Decremental operators:");
	output("--lhs: ", --_lhs);
	output("--lhs: ", --_lhs);
	output("--lhs: ", --_lhs);
	output("lhs--: ", _lhs--);
	output("lhs--: ", _lhs--);
	output("lhs--: ", _lhs--);
	nl();
}

void stringification(const auto lhs, const auto rhs) {
	output("Stringification:");
	output("lhs: ", lhs);
	output("lhs.to_string(): ", lhs.to_string());
	w_output("lhs.to_wstring(): ", lhs.to_wstring());
	output(std::format("std::format: {0}, {1}", lhs, rhs));
	nl();
}

void integral() {
	output("=== Integral ===");
	nl();

	STRICT_CPP_NAMESPACE::size_t lhs = 10ULL;
	STRICT_CPP_NAMESPACE::size_t rhs = 5ULL;

	output("Initial value of lhs (strict::size_t): ", lhs);
	output("Initial value of rhs (strict::size_t): ", rhs);
	nl();

	arithmetic_operators(lhs, rhs);

	unary_operators(lhs);

	binary_operators(lhs, rhs);

	comparison_operators(lhs, rhs);

	incremental_operators(lhs);

	decremental_operators(lhs);

	output("Conversion operators (static_cast):");

	output("strict::size_t as long: ", static_cast<long>(lhs));
	output("strict::size_t as int: ", static_cast<int>(lhs));
	output("strict::size_t as strict::count_t: ", static_cast<STRICT_CPP_NAMESPACE::count32_t>(lhs));
	output("strict::size_t as double: ", static_cast<double>(lhs));
	output("strict::size_t as strict::float_t ", static_cast<STRICT_CPP_NAMESPACE::float_t>(lhs));
	output("strict::size_t as std::size_t&: ", static_cast<std::size_t&>(lhs));
	nl();

	output("Conversion operators (c-style cast):");
	output("strict::size_t as long: ", (long)(lhs));
	output("strict::size_t as int: ", (int)(lhs));
	output("strict::size_t as strict::count_t: ", (STRICT_CPP_NAMESPACE::count32_t)(lhs));
	output("strict::size_t as double: ", (double)(lhs));
	output("strict::size_t as strict::float_t ", (STRICT_CPP_NAMESPACE::float_t)(lhs));
	output("strict::size_t as std::size_t&: ", (std::size_t&)lhs);
	nl();

	output("Conversion functions (.as<T>()):");
	output("strict::size_t as long: ", lhs.as<long>());
	output("strict::size_t as int: ", lhs.as<int>());
	output("strict::size_t as strict::count_t: ", lhs.as<STRICT_CPP_NAMESPACE::count32_t>());
	output("strict::size_t as double: ", lhs.as<double>());
	output("strict::size_t as strict::float_t: ", lhs.as<STRICT_CPP_NAMESPACE::float_t>());
	output("strict::size_t as std::size_t&: ", lhs.as<std::size_t&>());
	nl();

	stringification(lhs, rhs);
}

void floating_point() {
	output("=== Floats ===");
	nl();

	STRICT_CPP_NAMESPACE::float_t lhs = 10.0f;
	STRICT_CPP_NAMESPACE::float_t rhs = 5.0f;

	output("Initial value of lhs (strict::float_t): ", lhs);
	output("Initial value of rhs (strict::float_t): ", rhs);
	nl();

	arithmetic_operators(lhs, rhs);

	unary_operators(lhs);

	comparison_operators(lhs, rhs);

	incremental_operators(lhs);

	decremental_operators(lhs);

	output("Conversion operators (static_cast):");

	output("strict::float_t as long: ", static_cast<long>(lhs));
	output("strict::float_t as int: ", static_cast<int>(lhs));
	output("strict::float_t as strict::count_t: ", static_cast<STRICT_CPP_NAMESPACE::count32_t>(lhs));
	output("strict::float_t as double: ", static_cast<double>(lhs));
	output("strict::float_t as float&: ", static_cast<float&>(lhs));
	nl();

	output("Conversion operators (c-style cast):");

	output("strict::float_t as long: ", (long)(lhs));
	output("strict::float_t as int: ", (int)(lhs));
	output("strict::float_t as strict::count_t: ", (STRICT_CPP_NAMESPACE::count32_t)(lhs));
	output("strict::float_t as double: ", (double)(lhs));
	output("strict::float_t as float&: ", (float&)(lhs));
	nl();

	output("Conversion functions (.as<T>()):");

	output("strict::float_t as long: ", lhs.as<long>());
	output("strict::float_t as int: ", lhs.as<int>());
	output("strict::float_t as strict::count_t: ", lhs.as<STRICT_CPP_NAMESPACE::count32_t>());
	output("strict::float_t as double: ", lhs.as<double>());
	output("strict::float_t as float&: ", lhs.as<float&>());
	nl();

	stringification(lhs, rhs);
}

int main() {
	integral();
	floating_point();

	return 0;
}
