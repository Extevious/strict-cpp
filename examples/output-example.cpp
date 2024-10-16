#if !defined(STRICT_CPP_OPTIONAL_TYPES)
   #define STRICT_CPP_OPTIONAL_TYPES
#endif

#include "strict-cpp.hpp"

#include <iostream>

void output(const auto&... out) { (std::cout << ... << out) << '\n'; }

void output(const auto& msg, const bool out) { std::cout << msg << (out ? "true" : "false") << '\n'; }

void w_output(const auto&... out) { (std::wcout << ... << out) << '\n'; }

void nl() { std::cout << '\n'; }

void integral() {
   output("=== Integral ===");

   strict::size_t  a = 40ULL;
   strict::count_t b = strict::count_t(5.0f);

   output("a: ", a);
   output("b: ", b);
   nl();

   output("Arithmetic operators:");
   output("a + b: ", a + b);
   output("a - b: ", a - b);
   output("a * b: ", a * b);
   output("a / b: ", a / b);
   nl();

   output("Unary operators:");
   output("+a: ", +a);
   output("-a: ", -a);
   nl();

   output("Binary operators:");
   output("~a: ", ~a);
   output("a & b: ", a & b);
   output("a << b: ", a << b);
   output("a >> b: ", a >> b);
   output("a % b: ", a % b);
   output("a | b: ", a | b);
   output("a ^ b: ", a ^ b);
   nl();

   output("Compound assignment operators:");
   output("a += b: ", a += b);
   output("a -= b: ", a -= b);
   output("a *= b: ", a *= b);
   output("a /= b: ", a /= b);
   output("a &= b: ", a &= b);
   output("a <<= b: ", a <<= b);
   output("a >>= b: ", a >>= b);
   output("a %= b: ", a %= b);
   output("a |= b: ", a |= b);
   output("a ^= b: ", a ^= b);
   nl();

   output("Comparison operators:");
   output("a == b: ", a == b);
   output("a != b: ", a != b);
   output("a > b: ", a > b);
   output("a < b: ", a < b);
   output("a >= b: ", a >= b);
   output("a <= b: ", a <= b);
   nl();

   output("Increment operators:");
   output("a++: ", a++);
   output("a++: ", a++);
   output("a++: ", a++);
   output("++a: ", ++a);
   output("++a: ", ++a);
   output("++a: ", ++a);
   nl();

   output("Decrement operators:");
   output("a--: ", a--);
   output("--a: ", --a);
   nl();

   output("Casting operators:");

   std::size_t       s = a;
   int               i = (int)a;
   float             f = (float)a;
   strict::count32_t c = a.as<strict::count32_t>();

   output("s: ", s);
   output("i: ", i);
   output("f: ", f);
   output("c: ", c);
   nl();

   strict::schar_t schar0 = {};

   output("Small types (char, short, etc):");
   output("schar0: ", schar0);
   output("schar0 + 5: ", schar0 + 5);
   nl();

   output("Strings:");
   output("a: ", a);
   output("a.to_string(): ", a.to_string());
   w_output("a.to_wstring(): ", a.to_wstring());
   nl();
}

void floating() {
   output("=== Floating ===");

   strict::float_t a = 10.0f;
   strict::float_t b = strict::float_t(5.4l);

   output("Unary operators:");
   output("+a: ", +a);
   output("-a: ", -a);
   nl();

   output("Arithmetic operators:");
   output("a + b: ", a + b);
   output("a - b: ", a - b);
   output("a * b: ", a * b);
   output("a / b: ", a / b);
   output("a % b: ", a % b); // Uses std::fmod (C++23) or std::fmodf/l
   nl();

   output("Compound assignment operators:");
   output("a += b: ", a += b);
   output("a -= b: ", a -= b);
   output("a *= b: ", a *= b);
   output("a /= b: ", a /= b);
   output("a %= b: ", a %= b); // Uses std::fmod (C++23) or std::fmodf/l
   nl();

   output("Comparison operators:");
   output("a == b: ", a == b);
   output("a != b: ", a != b);
   output("a > b: ", a > b);
   output("a < b: ", a < b);
   output("a >= b: ", a >= b);
   output("a <= b: ", a <= b);
   nl();

   output("Increment operators:");
   output("a++: ", a++);
   output("a++: ", a++);
   output("a++: ", a++);
   output("++a: ", ++a);
   output("++a: ", ++a);
   output("++a: ", ++a);
   nl();

   output("Decrement operators:");
   output("a--: ", a--);
   output("--a: ", --a);
   nl();

   output("Casting operators:");

   float             f = a;
   std::size_t       s = (std::size_t)a;
   int               i = (int)a;
   strict::count32_t c = a.as<strict::count32_t>();

   output("f: ", f);
   output("s: ", s);
   output("i: ", i);
   output("c: ", c);
   nl();

   output("Strings:");
   output("a: ", a);
   output("a.to_string(): ", a.to_string());
   w_output("a.to_wstring(): ", a.to_wstring());
   nl();
}

int main() {
   integral();
   floating();

   return 0;
}
