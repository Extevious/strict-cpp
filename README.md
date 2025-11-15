# strict-cpp

Reduce ambiguity between methods and operators by using types provided by strict-cpp, a C++20 platform-agnostic project.

### Features

- Strict type wrappers encapsulating integral, floating-point, and alias types.
- Singular header file.
- Stringification and formatters.
- [Meson Build](https://mesonbuild.com/) support.
- MIT license.

### Usages

- Reduced ambiguity between methods and operators.
- Explicit type-safety enforcement.
- Method or operators will avoid swapped parameters, wrong parameter types, etc.

### TODO

- Verify compiled code has zero overhead in optimized builds.
- Verify cross-platform support.
- Implement support for [C++20 modules](https://en.cppreference.com/w/cpp/language/modules).
- CMake build file(s).
- ???

## Examples

### Integral and Floating-point types

```cpp

   // These could be ambiguous...
   add(short, short); // #1
   add(int, int);     // #2

   // ...but these are not!
   add(strict::int16_t, strict::int16_t); // #3
   add(strict::int32_t, strict::int32_t); // #4

   ...

   // If only method add() #1 and #2 existed:

   add(5, 6);   // This would call add() #2 as expected.
   add(5U, 6);  // This would also call add() #2 because of the 2nd argument qualifying.
   add(5U, 6U); // However this add() is ambiguous between #1 and #2.

   ...

   // If only the strict-type method add() #3 and #4 existed:

   add(5, 6);   // This would call add() #4 as expected.
   add(5U, 6);  // This would fail due to no implicit conversion available for the first argument.
   add(5U, 6U); // This would fail due to no implicit conversion available for both arguments.
```

### Custom types

```cpp
   // You can define your own types:
   //    - First parameter is the name of the type.
   //    - Second parameter is the encapsulated type.

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(strict::buffer_size_t, std::size_t);
   STRICT_CPP_DEFINE_FLOAT_TYPE(strict::scale_t, float);

   ...

   // You can also define your own types with a range of qualified types for implicit usage:
   //    - First parameter is the name of the type.
   //    - Second parameter is the encapsulated type.
   //    - All following types are the qualified types.

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(strict::example_t, int, long, double, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(strict::some_int_t, int, long, unsigned int);
```

### Integral and Floating-point dynamic types

```cpp
   // Functions can also use dynamic strict types.
   // someFunction() takes any signed integer, and the encapsulated type is explicitly an int.
   someFunction(strict::any_signed_int_t<int>); 

   // Using a template to define the encapsulated type when calling the function.
   // someFunction<T>() takes any float type, but the encapsulated type dynamically is T.
   template<typename T>
   someFunction(strict::any_float_t<T>);

   ...

   // Of course you can define your own dynamic type, but there is a slight difference:
   //    - First parameter is the name of the type.
   //    - All following types are the qualified types.

   STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(strict::any_example_t, char, int, std::int64_t);
```

### Alias types

```cpp
   // Defining a strict-alias type is easy:
   //    Note: if you need a comma (ex; for std::map<A,B>) the comma will need to be defined as a macro.
   STRICT_CPP_DEFINE_ALIAS_TYPE(some_string_A, std::string);
   STRICT_CPP_DEFINE_ALIAS_TYPE(some_string_B, std::string);

   ...

   void example0(strict::some_string_A string); // #1
   void example0(strict::some_string_B string); // #2

   const strict::some_string_A string0 = "some simple string A";
   const strict::some_string_B string1 = "some simple string B";
   const std::string           string2 = "I'm ambiguous!";

   example0(string0); // Calls #1
   example0(string1); // Calls #2

   // This is ambiguous due to the implicit conversion to both some_string_A or some_string_B types:
   example0(string2);

   ...

   // If you've ever had methods or constructors with similar or same parameters that differ
   // only in parameter name, using strict-aliases can significantly reduce ambiguity.

   // You might accidentally swap a and b:
   void example1(std::string a, std::string b);

   // But this is impossible to swap, unless you're passing the encapsulated type:
   void example1(strict::some_string_A a, strict::some_string_B b);
```

## How to include in your projects

### Cmake, Meson, or directly from GitHub

1. Clone or download this repo into your project.
2. Include the `strict-cpp.hpp` file that is inside the `include` directory.

### Meson (as a .wrap dependency)

1. Create a `strict-cpp.wrap` file in the `<project root>/subprojects` directory (if the directory doesn't exist, create one). An example `.wrap` file:

```cmake
   [wrap-git]
   url = https://github.com/Extevious/strict-cpp
   revision = head

   [provide]
   strict-cpp = strict_cpp_dependency
```

2. In your `meson.build` file, implement the following example:

```cmake
    # Your dependencies array.
    dependencies = []

    ...

    # The name of the subproject is the same as the filename.
    strict_cpp_subproject = subproject('strict-cpp')

    # Same as the name [provide] exposes in the .wrap file.
    dependencies += dependency('strict-cpp')
    #
    # Or...
    #
    # strict_cpp_dependency is provided by strict-cpp meson.build file.
    dependencies += strict_cpp_subproject.get_variable('strict_cpp_dependency')

    ...

    # Example executable.
    executable('MyProject', 'main.cpp', dependencies : strict_cpp_dependency)
```

3. `strict-cpp.hpp` should be available once your project has been updated.

## Macros

### Override-able

```cpp
// Default namespace name.
#define STRICT_CPP_NAMESPACE strict

// When defined, exposes pre-defined optional types.
#define STRICT_CPP_OPTIONAL_TYPES
```

### User-exposed

```cpp
// Macro to define your own integral types.
//    NAME            : the name of your strict type.
//    TYPE            : the encapsulated primitive type (int, long, etc).
//    QUALIFIED_TYPES : the optional range of types that are qualified for implicit usage.
#define STRICT_CPP_DEFINE_INTEGRAL_TYPE(NAME, TYPE, QUALIFIED_TYPES...)

// Macro to define your own floating-point types.
//    NAME            : the name of your strict type.
//    TYPE            : the encapsulated primitive type (float, double, etc).
//    QUALIFIED_TYPES : the optional range of types that are qualified for implicit usage.
#define STRICT_CPP_DEFINE_FLOAT_TYPE(NAME, TYPE, QUALIFIED_TYPES...)

// Macro to define your own dynamic integral types.
//    NAME            : the name of your strict type.
//    QUALIFIED_TYPES : the required range of types that are qualified for implicit usage.
#define STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(NAME, QUALIFIED_TYPES...)

// Macro to define your own dynamic floating-point types.
//    NAME            : the name of your strict type.
//    QUALIFIED_TYPES : the required range of types that are qualified for implicit usage.
#define STRICT_CPP_DEFINE_DYNAMIC_FLOAT_TYPE(NAME, QUALIFIED_TYPES...)

// Macro to define your own alias types.
//    NAME            : the name of your strict type.
//    TYPE            : the encapsulated type (std::string, std::vector<>, etc).
#define STRICT_CPP_DEFINE_ALIAS_TYPE(NAME, TYPE)
```

## Available pre-defined types

```cpp
   Name:                  Encapsulated Type:
    char_t                 char
    schar_t                signed char
    uchar_t                unsigned char
    short_t                short
    ushort_t               unsigned short
    int_t                  int
    uint_t                 unsigned int
    long_t                 long
    ulong_t                unsigned long

    size_t                 std::size_t
    size8_t                std::uint8_t
    size16_t               std::uint16_t
    size32_t               std::uint32_t
    size64_t               std::uint64_t

    int8_t                 std::int8_t
    int16_t                std::int16_t
    int32_t                std::int32_t
    int64_t                std::int64_t

    int_least_t            std::int_least32_t
    int_least8_t           std::int_least8_t
    int_least16_t          std::int_least16_t
    int_least32_t          std::int_least32_t
    int_least64_t          std::int_least64_t
    
    int_fast_t             std::int_fast32_t
    int_fast8_t            std::int_fast8_t
    int_fast16_t           std::int_fast16_t
    int_fast32_t           std::int_fast32_t
    int_fast64_t           std::int_fast64_t

    uint8_t                std::uint8_t
    uint16_t               std::uint16_t
    uint32_t               std::uint32_t
    uint64_t               std::uint64_t

    uint_least_t           std::uint_least32_t
    uint_least8_t          std::uint_least8_t
    uint_least16_t         std::uint_least16_t
    uint_least32_t         std::uint_least32_t
    uint_least64_t         std::uint_least64_t

    uint_fast_t            std::uint_fast32_t
    uint_fast8_t           std::uint_fast8_t
    uint_fast16_t          std::uint_fast16_t
    uint_fast32_t          std::uint_fast32_t
    uint_fast64_t          std::uint_fast64_t

    intmax_t               std::intmax_t
    uintmax_t              std::uintmax_t
    intptr_t               std::intptr_t
    uintptr_t              std::uintptr_t
    byte_t                 std::uint8_t
    sbyte_t                std::int8_t

    any_int_t<T>           ...
    any_size_t<T>          ...
    any_signed_int_t<T>    ...
    any_unsigned_int_t<T>  ...
    any_int_least_t<T>     ...
    any_int_fast_t<T>      ...
    any_uint_least_t<T>    ...
    any_uint_fast_t<T>     ...
    any_intmax_t<T>        ...
    any_intptr_t<T>        ...
    any_byte_t<T>          ...
```

```cpp
    float_t                float
    double_t               double
    long_double_t          long double
    float32_t              std::float_t
    float64_t              std::double_t

    any_float_t<T>         ...
```

## Available pre-defined optional types

***Note:*** *optional types available by defining the* `STRICT_CPP_OPTIONAL_TYPES` *macro.*

```cpp
   Name:                  Encapsulated type:
    offset_size_t          std::size_t
    begin_size_t           std::size_t
    begin_offset_size_t    std::size_t
    end_size_t             std::size_t
    end_offset_size_t      std::size_t
    src_size_t             std::size_t
    src_offset_size_t      std::size_t
    dst_size_t             std::size_t
    dst_offset_size_t      std::size_t
    lhs_size_t             std::size_t
    lhs_offset_size_t      std::size_t
    rhs_size_t             std::size_t
    rhs_offset_size_t      std::size_t
    count_t                std::size_t
    offset_count_t         std::size_t
    src_count_t            std::size_t
    src_offset_count_t     std::size_t
    begin_count_t          std::size_t
    begin_offset_count_t   std::size_t
    lhs_count_t            std::size_t
    lhs_offset_count_t     std::size_t
    dst_count_t            std::size_t
    dst_offset_count_t     std::size_t
    end_count_t            std::size_t
    end_offset_count_t     std::size_t
    rhs_count_t            std::size_t
    rhs_offset_count_t     std::size_t
    offset_t               std::size_t
    begin_offset_t         std::size_t
    end_offset_t           std::size_t
    src_offset_t           std::size_t
    dst_offset_t           std::size_t
    lhs_offset_t           std::size_t
    rhs_offset_t           std::size_t
    index_t                std::size_t
    index_offset_t         std::size_t
    begin_index_t          std::size_t
    begin_index_offset_t   std::size_t
    end_index_t            std::size_t
    end_index_offset_t     std::size_t
    src_index_t            std::size_t
    src_index_offset_t     std::size_t
    dst_index_t            std::size_t
    dst_index_offset_t     std::size_t
    lhs_index_t            std::size_t
    lhs_index_offset_t     std::size_t
    rhs_index_t            std::size_t
    rhs_index_offset_t     std::size_t
    capacity_t             std::size_t
```

### *... and more*
