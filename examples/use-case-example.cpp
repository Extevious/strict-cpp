#if !defined(STRICT_CPP_OPTIONAL_TYPES)
   #define STRICT_CPP_OPTIONAL_TYPES
#endif

#include "strict-cpp.hpp"

#include <iostream>

void output(const auto&... out) { (std::cout << ... << out) << '\n'; }

// === Addition ===

// #1
void add(STRICT_CPP_NAMESPACE::int32_t a, STRICT_CPP_NAMESPACE::int32_t b) { output("add() #1 was called: ", a + b); }

// #2
void add(STRICT_CPP_NAMESPACE::int32_t a, STRICT_CPP_NAMESPACE::uint32_t b) { output("add() #2 was called: ", a + b); }

// #3
void add(STRICT_CPP_NAMESPACE::int64_t a, STRICT_CPP_NAMESPACE::int32_t b) { output("add() #3 was called: ", a + b); }

// #4
void add(STRICT_CPP_NAMESPACE::int8_t a, STRICT_CPP_NAMESPACE::int8_t b) { output("add() #4 was called: ", a + b); }

// #5
void add(STRICT_CPP_NAMESPACE::int16_t a, short b) { output("add() #5 was called: ", a + b); }

// === Subtraction ===

// #1
void subtract(STRICT_CPP_NAMESPACE::size_t a, STRICT_CPP_NAMESPACE::size_t b) { output("subtract() #1 was called: ", a - b); }

// #2
void subtract(STRICT_CPP_NAMESPACE::count_t a, STRICT_CPP_NAMESPACE::count_t b) { output("subtract() #2 was called: ", a - b); }

// #3
void subtract(STRICT_CPP_NAMESPACE::count_t a, STRICT_CPP_NAMESPACE::size_t b) { output("subtract() #3 was called: ", a - b); }

// === Multiplication ===

// #1
void multiply(STRICT_CPP_NAMESPACE::float_t a, STRICT_CPP_NAMESPACE::float_t b) { output("multiply() #1 was called: ", a * b); }

// #2
void multiply(STRICT_CPP_NAMESPACE::float_t a, STRICT_CPP_NAMESPACE::double_t b) { output("multiply() #2 was called: ", a * b); }

// #3
void multiply(STRICT_CPP_NAMESPACE::long_double_t a, STRICT_CPP_NAMESPACE::double_t b) { output("multiply() #3 was called: ", a * b); }

// #4
void multiply(STRICT_CPP_NAMESPACE::float_t a, STRICT_CPP_NAMESPACE::size_t b) { output("multiply() #4 was called: ", a * b); }

// === Main ===

int main() {
   // strict signed int
   STRICT_CPP_NAMESPACE::int32_t a = 5;

   // std signed int
   std::int32_t b = -34;

   // Implicit conversions are possible for the encapsulated-type only, for example:
   add(45, 56);                                  // Calls add #1
   add(a, 4);                                    // Calls add #1
   add(a, b);                                    // Calls add #1
   add(a, 5U);                                   // Calls add #2
   add(STRICT_CPP_NAMESPACE::int64_t(5.0f), 22); // Calls add #3
   add((std::int8_t)41, (std::int8_t)65);        // Calls add #4
   add((short)a, (short)12);                     // Calls add #5

   // This doesn't work because they're both unsigned ints as there is no overload available:
   //
   //    add(5U, 9U);
   //

   // Explicit constructors can be called so long as the type being passed to the constructor can be converted:
   add(STRICT_CPP_NAMESPACE::int32_t(5U), STRICT_CPP_NAMESPACE::int32_t(9U)); // Calls #1

   // Unfortunately, there is some overlap between types when it comes to implicit
   // conversions due to both STRICT_CPP_NAMESPACE::size_t and STRICT_CPP_NAMESPACE::count_t having the same
   // encapsulated type (std::size_t) resulting in ambiguity between #1, #2, and #3.
   // To get around this, cast to the desired strict type:
   subtract((STRICT_CPP_NAMESPACE::size_t)234ULL, 78ULL);                                // Calls subtract #1
   subtract(234ULL, (STRICT_CPP_NAMESPACE::count_t)78ULL);                               // Calls subtract #2
   subtract((STRICT_CPP_NAMESPACE::count_t)234ULL, (STRICT_CPP_NAMESPACE::size_t)78ULL); // Calls subtract #3

   // Floats are supported like you'd normally expect them to work:
   multiply(1.0f, 6.34f);                                // Calls multiply() #1
   multiply(3.5f, STRICT_CPP_NAMESPACE::double_t(6.34)); // Calls multiply() #2
   multiply(2.0L, (STRICT_CPP_NAMESPACE::double_t)6.34); // Calls multiply() #3
   multiply(2.0f, STRICT_CPP_NAMESPACE::size_t(45));     // Calls multiply() #4

   // Strict types can still be used on pointers like you normally would:
   constexpr const char* someMessage = "I'm a totally cool message..!";

   const char* character = someMessage + STRICT_CPP_NAMESPACE::size_t(9); // Character: a

   output("Character: ", *character);

   character += STRICT_CPP_NAMESPACE::size_t(10); // Character: m

   output("Character: ", *character);

   // to_string functions are already provided, making stringification easy:
   output(a.to_string());

   // You can also pass strict types into a formatter:
   output(std::format("I have some numbers: {0}, {1}, {2}", a, b, 12345));

   return 0;
}

// You can define your own types:
STRICT_CPP_DEFINE_INTEGRAL_TYPE(buffer_size_t, std::size_t);
STRICT_CPP_DEFINE_FLOAT_TYPE(scale_t, float);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(some_totally_cool_type_t, long);

// You can define your own types with multiple qualified types, (the second type being the encapsulated type):
STRICT_CPP_DEFINE_INTEGRAL_TYPE(some_ints_t, int, unsigned int, long, std::size_t);
STRICT_CPP_DEFINE_FLOAT_TYPE(some_floats_t, float, double, long double, STRICT_CPP_NAMESPACE::float64_t);

// Which means you can do stuff like this:
// #1
void customTypeExample(const STRICT_CPP_NAMESPACE::some_floats_t v) { output("customTypeExample() #1 was called: ", v); }

// #2
void customTypeExample(const STRICT_CPP_NAMESPACE::some_ints_t v) { output("customTypeExample() #2 was called: ", v); }

// #3
void customTypeExample(const STRICT_CPP_NAMESPACE::size_t v) { output("customTypeExample() #3 was called: ", v); } // Overloaded to demonstrate ambiguity.

// Dynamic types are handy for when you have a small selection of functions
// or operator overloads that are unique enough that even with a range of qualified types wont be ambiguous,
// but you want to specify the encapsulated type:

// #1
void explicitDynamicTypeExample(const STRICT_CPP_NAMESPACE::any_size_t<std::uint16_t> v) { output("explicitDynamicTypeExample() #1 was called: ", v); }

// #2
void explicitDynamicTypeExample(const STRICT_CPP_NAMESPACE::any_int_t<long long> v) { output("explicitDynamicTypeExample() #2 was called: ", v); }

// #1
template <typename T>
void dynamicTypeExample(const STRICT_CPP_NAMESPACE::any_int_t<T> v) {
   output("dynamicTypeExample<T>() #1 was called: ", v);
}

// #2
template <typename T>
void dynamicTypeExample(const STRICT_CPP_NAMESPACE::any_float_t<T> v) {
   output("dynamicTypeExample<T>() #2 was called: ", v);
}

void test() {
   customTypeExample(5.0F); // Calls customTypeExample() #1
   customTypeExample(1.0L); // Calls customTypeExample() #1
   customTypeExample(3.);   // Calls customTypeExample() #1

   customTypeExample(5);   // Calls customTypeExample() #2
   customTypeExample(10U); // Calls customTypeExample() #2
   customTypeExample(25L); // Calls customTypeExample() #2

   // Ambiguous as implicit conversion is possible for both some_ints_t and STRICT_CPP_NAMESPACE::size_t.
   //
   //    customTypeExample(789ULL);
   //

   explicitDynamicTypeExample(STRICT_CPP_NAMESPACE::size_t(20)); // Calls explicitDynamicTypeExample() #1
   explicitDynamicTypeExample(STRICT_CPP_NAMESPACE::size8_t(5)); // Calls explicitDynamicTypeExample() #1
   explicitDynamicTypeExample(5);                                // Calls explicitDynamicTypeExample() #2

   // This is ambiguous because there's an implicit conversion possible for both any_size_t and any_int_t.
   //
   //    explicitDynamicTypeExample(4ULL);
   //

   // Calling a function with a dynamically-qualified type is easy:
   dynamicTypeExample<long>(34);
   dynamicTypeExample<double>(500.0f);

   // This doesn't work due to the encapsulated type being an integer and the provided type being a float.
   //
   //    dynamicTypeExample<long>(123.0f);
   //
}
