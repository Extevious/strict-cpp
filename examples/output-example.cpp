#if !defined(STRICT_CPP_OPTIONAL_TYPES)
   #define STRICT_CPP_OPTIONAL_TYPES
#endif

#include "strict-cpp.hpp"

#include <iostream>

void output(auto out) { std::cout << out << '\n'; }

void w_output(auto out) { std::wcout << out << '\n'; }

void nl() { std::cout << '\n'; }

void integral() {
   output("=== Integral ===");

   strict::size_t  a = 42ULL;
   strict::count_t b = strict::count_t(5.0f);

   output("Arithmetic operators:");
   output(a + b);
   output(a - b);
   output(a * b);
   output(a / b);
   nl();

   output("Unary operators:");
   output(+a);
   output(-a);
   nl();

   output("Binary operators:");
   output(~a);
   output(a & b);
   output(a << b);
   output(a >> b);
   output(a % b);
   output(a | b);
   output(a ^ b);
   nl();

   output("Compound assignment operators:");
   output(a += b);
   output(a -= b);
   output(a *= b);
   output(a /= b);
   output(a &= b);
   output(a <<= b);
   output(a >>= b);
   output(a %= b);
   output(a |= b);
   output(a ^= b);
   nl();

   output("Comparison operators:");
   output(a == b);
   output(a != b);
   output(a > b);
   output(a < b);
   output(a >= b);
   output(a <= b);
   nl();

   output("Increment operators:");
   output(a++);
   output(++a);
   nl();

   output("Decrement operators:");
   output(a--);
   output(--a);
   nl();

   output("Casting operators:");

   std::size_t       s = a;
   int               i = (int)a;
   float             f = (float)a;
   strict::count32_t c = (strict::count32_t)a;

   output(s);
   output(i);
   output(f);
   output(c);
   nl();

   output("Strings:");
   output(a);
   output(a.to_string());
   output(a.to_string_t());
   output(a.to_string_n());
   w_output(a.to_wstring());
   w_output(a.to_wstring_t());
   w_output(a.to_wstring_n());
   nl();
}

void floating() {
   output("=== Floating ===");

   strict::float_t a = 16.854f;
   strict::float_t b = strict::float_t(5.4l);

   output("Unary operators:");
   output(+a);
   output(-a);
   nl();

   output("Arithmetic operators:");
   output(a + b);
   output(a - b);
   output(a * b);
   output(a / b);
   output(a % b); // Uses std::fmod (C++23) or std::fmodf/l
   nl();

   output("Compound assignment operators:");
   output(a += b);
   output(a -= b);
   output(a *= b);
   output(a /= b);
   output(a %= b); // Uses std::fmod (C++23) or std::fmodf/l
   nl();

   output("Comparison operators:");
   output(a == b);
   output(a != b);
   output(a > b);
   output(a < b);
   output(a >= b);
   output(a <= b);
   nl();

   output("Increment operators:");
   output(a++);
   output(++a);
   nl();

   output("Decrement operators:");
   output(a--);
   output(--a);
   nl();

   output("Casting operators:");

   float             f = a;
   std::size_t       s = (std::size_t)a;
   int               i = (int)a;
   strict::count32_t c = (strict::count32_t)a;

   output(f);
   output(s);
   output(i);
   output(c);
   nl();

   output("Strings:");
   output(a);
   output(a.to_string());
   output(a.to_string_t());
   output(a.to_string_n());
   w_output(a.to_wstring());
   w_output(a.to_wstring_t());
   w_output(a.to_wstring_n());
   nl();
}

int main() {
   integral();
   floating();

   return 0;
}
