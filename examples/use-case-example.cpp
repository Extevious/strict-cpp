#define STRICT_CPP_OPTIONAL_TYPES

#include "strict-cpp.hpp"

#include <cstdint>
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

// Notice how the only difference from the above add() is the similar types.
// Without using the strict types, add() calls would be ambiguous since
// converting std::int32_t to std::int8_t is implicit.
void add(strict::int8_t a, strict::int8_t b) { output("add() #2: ", a + b); }

// Adding of an int8 and int32.
// Note: using strict type operators requires the types to match.
void add(strict::int64_t a, strict::int32_t b) { output("add() #3: ", a + (strict::int64_t)b); }

// Adding of the implicit same type is still supported.
void add(strict::int16_t a, short b) { output("add() #4: ", a + b); }

void subtract(strict::size_t a, strict::size_t b) { output("subtract() #1: ", a - b); }

void subtract(strict::count_t a, strict::count_t b) { output("subtract() #2: ", a - b); }

int main() {
   add(4, 5);

   std::int32_t a = 5;
   std::int32_t b = -34;

   add(a, b);
   add(a, 4);
   add(std::int64_t(5.0f), 22);

   // This doesn't work because they're unsigned ints:
   //
   //    add(5U, 9U);
   //
   // ...but this works because the constructors are being called explicitly.
   add(std::int32_t(5U), std::int32_t(9U));

   // Unfortunately, there is some overlap between types when it comes to implicit
   // conversions. Both strict::size_t and strict::count_t have the same
   // encapsulated type (std::size_t), which results in ambiguity.
   //
   //   subtract(234ULL, 78ULL);
   //
   // To get around this issue, simply convert to the desired type:
   subtract(strict::size_t(234ULL), (strict::size_t)78ULL);

   return 0;
}

// You can also define your own types:
STRICT_CPP_DEFINE_INTEGRAL_TYPE(buffer_size_t, std::size_t);
STRICT_CPP_DEFINE_FLOAT_TYPE(scale_t, float);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(some_totally_cool_type_t, long);
