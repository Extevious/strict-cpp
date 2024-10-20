#if !defined(STRICT_CPP_OPTIONAL_TYPES)
   #define STRICT_CPP_OPTIONAL_TYPES
#endif

#include "strict-cpp.hpp"

#include <iostream>

void output(const auto&... out) { (std::cout << ... << out) << '\n'; }

// === Addition ===

// #1
void add(strict::int32_t a, strict::int32_t b) { output("add() #1: ", a + b); }

// #2
void add(strict::int32_t a, strict::uint32_t b) { output("add() #2: ", a + b); }

// #3
void add(strict::int64_t a, strict::int32_t b) { output("add() #3: ", a + b); }

// #4
void add(strict::int8_t a, strict::int8_t b) { output("add() #4: ", a + b); }

// #5
void add(strict::int16_t a, short b) { output("add() #5: ", a + b); }

// === Subtraction ===

// #6
void subtract(strict::size_t a, strict::size_t b) { output("subtract() #6: ", a - b); }

// #7
void subtract(strict::count_t a, strict::count_t b) { output("subtract() #7: ", a - b); }

// #8
void subtract(strict::count_t a, strict::size_t b) { output("subtract() #8: ", a - b); }

// === Multiplication ===

// #9
void multiply(strict::float_t a, strict::float_t b) { output("multiply() #9: ", a * b); }

// #10
void multiply(strict::float_t a, strict::double_t b) { output("multiply() #10: ", a * b); }

// #11
void multiply(strict::long_double_t a, strict::double_t b) { output("multiply() #11: ", a * b); }

// #12
void multiply(strict::float_t a, strict::size_t b) { output("multiply() #12: ", a * b); }

// === Main ===

int main() {
   // strict signed int
   strict::int32_t a = 5;

   // std signed int
   std::int32_t b = -34;

   // Implicit conversions are possible for the encapsulated-type only, for example:
   add(a, b);                             // Calls #1
   add(a, 4);                             // Calls #1
   add(45, 56);                           // Calls #1
   add(a, 5U);                            // Calls #2
   add(strict::int64_t(5.0f), 22);        // Calls #3
   add((std::int8_t)41, (std::int8_t)65); // Calls #4
   add((short)a, (short)12);              // Calls #5

   // This doesn't work because they're both unsigned ints as there is no overload available:
   //
   //    add(5U, 9U);
   //
   // ...but this works because the constructors are being called explicitly.
   add(strict::int32_t(5U), strict::int32_t(9U)); // Calls #1

   // Unfortunately, there is some overlap between types when it comes to implicit
   // conversions due to both strict::size_t and strict::count_t having the same
   // encapsulated type (std::size_t) resulting in ambiguity between #6, #7, and #8.
   //
   //   subtract(234ULL, 78ULL);
   //
   // To get around this issue, simply convert to the desired type:
   subtract((strict::size_t)234ULL, 78ULL);                  // Calls #6
   subtract(234ULL, (strict::count_t)78ULL);                 // Calls #7
   subtract((strict::count_t)234ULL, (strict::size_t)78ULL); // Calls #8

   // Floats are supported like you'd normally expect them to work:
   multiply(1.0f, 6.34f);                  // Calls #9
   multiply(3.5f, strict::double_t(6.34)); // Calls #10
   multiply(2.0L, (strict::double_t)6.34); // Calls #11
   multiply(2.0f, strict::size_t(45));     // Calls #12

   // Strict types can still be used on pointers like you normally would:
   constexpr const char* someMessage = "I'm a totally cool message..!";

   const char* character = someMessage + strict::size_t(9); // Character: a

   output("Character: ", *character);

   character += strict::size_t(10); // Character: m

   output("Character: ", *character);

   // to_string functions are already provided, making stringification easy:
   output(a.to_string());

   return 0;
}

// You can also define your own types:
STRICT_CPP_DEFINE_INTEGRAL_TYPE(buffer_size_t, std::size_t);
STRICT_CPP_DEFINE_FLOAT_TYPE(scale_t, float);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(some_totally_cool_type_t, long);

// You can specify multiple qualified types, with the first type being the encapsulated type:
STRICT_CPP_DEFINE_INTEGRAL_TYPE(some_ints_t, int, unsigned int, long, std::size_t);
STRICT_CPP_DEFINE_FLOAT_TYPE(some_floats_t, float, double, long double);

// Which means you can do stuff like this:
// #1
void example0(some_floats_t) { }

// #2
void example0(some_ints_t) { }

// #3
void example0(strict::size_t) { } // Overloaded to demonstrate ambiguity.

// #4
void example1(strict::any_size_t) { }

// #5
void example1(strict::any_int_t) { }

void test() {
   // example0:
   example0(5.0F); // Calls #1
   example0(1.0L); // Calls #1
   example0(3.);   // Calls #1

   // example1:
   example0(5);   // Calls #2
   example0(10U); // Calls #2
   example0(25L); // Calls #2

   // Ambiguous as implicit conversion is possible for both some_ints_t and strict::size_t.
   // example0(789ULL);

   // Some pre-defined "any" types exist:
   example1(strict::size_t(20)); // Calls #4
   example1(strict::size8_t(5)); // Calls #4
   example1(5);                  // Calls #5

   // Ambiguous as implicit conversion is possible for both any_size_t and any_int_t.
   // example1(4ULL);
}
