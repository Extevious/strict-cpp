#define STRICT_CPP_OPTIONAL_TYPES

#include "strict-cpp.hpp"

#include <iostream>

void output(auto... out) { (std::cout << ... << out) << '\n'; }

// This gets called instead of the other add() methods because of implicit
// conversion. This is to allow compatibility with existing APIs.
//
//       void add(int a, int b) { }
//

// A compiler error is thrown claiming this is a redefinition since the conversion
// between int and unsigned int is implicit.
//
//       void add(uint a, uint b) { }
//

// A simple method adding 2 signed ints together.
void add(strict::int32_t a, strict::int32_t b) { output("add() #1: ", a + b); }

// Similar to above, but b is unsigned.
void add(strict::int32_t a, strict::uint32_t b) { output("add() #2: ", a + b); }

// Notice how the only difference from the above add() is the similar types.
// Without using the strict types, add() calls would be ambiguous since
// converting std::int32_t to std::int8_t is implicit.
void add(strict::int8_t a, strict::int8_t b) { output("add() #3: ", a + b); }

// Adding of an int64 and int32.
void add(strict::int64_t a, strict::int32_t b) { output("add() #4: ", a + b); }

// Adding of the implicit same type is still supported.
void add(strict::int16_t a, short b) { output("add() #5: ", a + b); }

void subtract(strict::size_t a, strict::size_t b) { output("subtract() #1: ", a - b); }

void subtract(strict::count_t a, strict::count_t b) { output("subtract() #2: ", a - b); }

// Simple multiplication of 2 floats.
void multiply(strict::float_t a, strict::float_t b) { output("multiply() #1: ", a * b); }

// Regardless of order, ((a * b) vs (b * a)) the smaller type gets promoted.
void multiply(strict::float_t a, strict::double_t b) { output("multiply() #2: ", a * b); }

// The resulting type is strict::long_double_t after multiplying a * b.
void multiply(strict::long_double_t a, strict::double_t b) { output("multiply() #3: ", a * b); }

// Multiplying a float with a strict::size_t.
void multiply(strict::float_t a, strict::size_t b) { output("multiply() #4: ", a * b); }

int main() {
   // strict signed int
   strict::int32_t a = 5;

   // std signed int
   std::int32_t b = -34;

   // Calls #1
   add(a, b);

   // Calls #1
   add(a, 4);

   // Calls #2
   add(a, 5U);

   // Calls #5
   add(strict::int16_t(a), (short)12);

   // Calls #3
   add((std::int8_t)41, (std::int8_t)65);

   // Calls #4
   add(strict::int64_t(5.0f), 22);

   // This doesn't work because they're both unsigned ints:
   //
   //    add(5U, 9U);
   //
   // ...but this works because the constructors are being called explicitly.
   add(strict::int32_t(5U), strict::int32_t(9U));

   // Unfortunately, there is some overlap between types when it comes to implicit
   // conversions. Both strict::size_t and strict::count_t have the same
   // encapsulated type (std::size_t), which results in ambiguity.
   //
   //   subtract(234ULL, 78ULL);
   //
   // To get around this issue, simply convert to the desired type:
   subtract((strict::size_t)234ULL, (strict::size_t)78ULL);

   // Floats are supported like you'd normally expect them to work:

   // Calls #1
   multiply(1.0f, 6.34f);

   // Calls #2
   multiply(3.5f, strict::double_t(6.34));

   // Calls #3
   multiply(2.0L, (strict::double_t)6.34);

   // Calls #4
   multiply(2.0f, strict::size_t(45));

   // Strict types can still be used on pointers like you normally would:
   constexpr const char* someMessage = "I'm a totally cool message..!";

   const char* ptr = someMessage;

   // Character: I
   output("Character: ", *ptr);

   ptr += strict::size_t(9);

   // Character: a
   output("Character: ", *ptr);

   // Character: m
   output("Character: ", *(ptr + strict::size_t(10)));

   return 0;
}

// You can also define your own types (but it has to be in the STRICT_CPP_NAMESPACE namespace):
namespace STRICT_CPP_NAMESPACE {
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(buffer_size_t, std::size_t);
   STRICT_CPP_DEFINE_FLOAT_TYPE(scale_t, float);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(some_totally_cool_type_t, long);
}
