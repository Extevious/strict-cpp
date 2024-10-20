## strict-cpp

Reduce ambiguity between methods and operators by using types provided by strict-cpp, a C++20 platform-agnostic project.

### Features:

- Strict type wrappers encapsulating integral, floating-point, and alias types.
- Singular header file.
- [Meson Build](https://mesonbuild.com/) support.
- MIT license.

### Can be used to...:

- Reduce ambiguity between methods and operators.
- Enforce more explicit type-safety.
- Use method or operator overloads more freely with less ambiguity.

### Example:

```cpp
   // =======================================================================
   // Integral and Floating-point types
   // =======================================================================

   // These could be ambiguous...
   add(short, short); // #1
   add(int, int);     // #2

   // ...but these are not!
   add(strict::int16_t, strict::int16_t); // #3
   add(strict::int32_t, strict::int32_t); // #4

   ...

   // If only the add() methods #1 and #2 existed:

   add(5, 6);   // This would call add() #2 as expected.
   add(5U, 6);  // This would also call add() #2 because of the 2nd argument qualifying.
   add(5U, 6U); // However this add() is ambiguous between #1 and #2.

   // If only the strict-type add() methods #3 and #4 existed:

   add(5, 6);   // This would call add() #4 as expected.
   add(5U, 6);  // This would fail due to no implicit conversion available for the first argument.
   add(5U, 6U); // This would fail due to no implicit conversion available for both arguments.

   ...

   // You can define your own types:
   //    - First parameter is the name of the type.
   //    - Second parameter is the type.

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(buffer_size_t, std::size_t);
   STRICT_CPP_DEFINE_FLOAT_TYPE(scale_t, float);

   // You can also define your own types with a range of qualified types for implicit usage:
   //    - First parameter is the name of the type.
   //    - Second parameter is the type.
   //    - All following types are the qualified types.

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(example_t, int, long, double, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(some_int_t, int, long, unsigned int);

   ...

   // If you defined a method that has an example_t as a parameter you could do:
   myMethod(5);
   myMethod(60L);
   myMethod(23U);
```

```cpp
   // =======================================================================
   // Alias types
   // =======================================================================

   // Defining an alias type is easy:
   //    Note: if you need a comma (ex; for std::map<A,B>) it will need to be defined as a macro.
   STRICT_CPP_DEFINE_ALIAS_TYPE(some_string_A, std::string);
   STRICT_CPP_DEFINE_ALIAS_TYPE(some_string_B, std::string);

   ...

   void example0(const some_string_A& string); // #1
   void example0(const some_string_B& string); // #2

   const some_string_A string0 = "some simple string A";
   const some_string_B string1 = "some simple string B";
   const std::string   string2 = "I'm ambiguous!";

   example0(string0); // Calls #1
   example0(string1); // Calls #2

   // This is ambiguous due to the implicit conversion to both some_string_A or some_string_B types:
   example0(string2);

   ...

   // If you've ever had methods or constructors with similar or same parameters that differ
   // only in parameter name, using strict-aliases can significantly reduce that ambiguity.

   // You might accidentally swap a and b:
   void example1(const std::string& a, const std::string& b);

   // But this is impossible to swap, unless you're implicitly converting from the encapsulated type:
   void example1(const some_string_A& a, const some_string_B& b);
```

## How to include in your projects

### Cmake or Meson

1. Clone or download this repo into your project.
2. Include the `strict-cpp.hpp` file that is inside the `include` directory.

### Meson (as a .wrap dependency)

1. Create a `strict-cpp.wrap` file in the `<project root>/subprojects` directory. Example `.wrap` file:

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

### Override-able:

```cpp
// Default namespace name.
#define STRICT_CPP_NAMESPACE strict

// Expose optional pre-defined types.
#define STRICT_CPP_OPTIONAL_TYPES
```

### User-exposed:

```cpp
// Macro to define your own integral types.
//    NAME            : the name of your strict type.
//    TYPE            : the encapsulated primitive type (float, double, etc).
//    QUALIFIED_TYPES : the optional range of types that are qualified for implicit usage.
#define STRICT_CPP_DEFINE_INTEGRAL_TYPE(NAME, TYPE, QUALIFIED_TYPES...)

// Macro to define your own floating-point types.
//    NAME            : the name of your strict type.
//    TYPE            : the encapsulated primitive type (float, double, etc).
//    QUALIFIED_TYPES : the optional range of types that are qualified for implicit usage.
#define STRICT_CPP_DEFINE_FLOAT_TYPE(NAME, TYPE, QUALIFIED_TYPES...)

// Macro to define your own alias types.
//    NAME            : the name of your strict type.
//    TYPE            : the encapsulated type (std::string, std::vector<>, etc).
#define STRICT_CPP_DEFINE_ALIAS_TYPE(NAME, TYPE)
```

## Available types

### Common:

```cpp
// Integral types
size_t               (std::size_t)

int8_t               (std::int8_t)
int16_t              (std::int16_t)
int32_t              (std::int32_t)
int64_t              (std::int64_t)

int_least8_t         (std::int_least8_t)
int_least16_t        (std::int_least16_t)
int_least32_t        (std::int_least32_t)
int_least64_t        (std::int_least64_t)

int_fast8_t          (std::int_fast8_t)
int_fast16_t         (std::int_fast16_t)
int_fast32_t         (std::int_fast32_t)
int_fast64_t         (std::int_fast64_t)

uint8_t              (std::uint8_t)
uint16_t             (std::uint16_t)
uint32_t             (std::uint32_t)
uint64_t             (std::uint64_t)

uint_least8_t        (std::uint_least8_t)
uint_least16_t       (std::uint_least16_t)
uint_least32_t       (std::uint_least32_t)
uint_least64_t       (std::uint_least64_t)

uint_fast8_t         (std::uint_fast8_t)
uint_fast16_t        (std::uint_fast16_t)
uint_fast32_t        (std::uint_fast32_t)
uint_fast64_t        (std::uint_fast64_t)

intmax_t             (std::intmax_t)
uintmax_t            (std::uintmax_t)

intptr_t             (std::intptr_t)
uintptr_t            (std::uintptr_t)

char_t               (char)
schar_t              (signed char)
uchar_t              (unsigned char)

// Floating-point types
float_t              (float)
double_t             (double)
long_double_t        (long double)
```

### Optional:

```cpp
// Integral types
size8_t              (std::uint8_t)
size16_t             (std::uint16_t)
size32_t             (std::uint32_t)
size64_t             (std::uint64_t)

src_size_t           (std::size_t)
src_size8_t          (std::uint8_t)
src_size16_t         (std::uint16_t)
src_size32_t         (std::uint32_t)
src_size64_t         (std::uint64_t)

src_offset_size_t    (std::size_t)
src_offset_size8_t   (std::uint8_t)
src_offset_size16_t  (std::uint16_t)
src_offset_size32_t  (std::uint32_t)
src_offset_size64_t  (std::uint64_t)

dst_size_t           (std::size_t)
dst_size8_t          (std::uint8_t)
dst_size16_t         (std::uint16_t)
dst_size32_t         (std::uint32_t)
dst_size64_t         (std::uint64_t)

dst_offset_size_t    (std::size_t)
dst_offset_size8_t   (std::uint8_t)
dst_offset_size16_t  (std::uint16_t)
dst_offset_size32_t  (std::uint32_t)
dst_offset_size64_t  (std::uint64_t)

capacity_t           (std::size_t)
capacity8_t          (std::uint8_t)
capacity16_t         (std::uint16_t)
capacity32_t         (std::uint32_t)
capacity64_t         (std::uint64_t)

count_t              (std::size_t)
count8_t             (std::uint8_t)
count16_t            (std::uint16_t)
count32_t            (std::uint32_t)
count64_t            (std::uint64_t)

offset_count_t       (std::size_t)
offset_count8_t      (std::uint8_t)
offset_count16_t     (std::uint16_t)
offset_count32_t     (std::uint32_t)
offset_count64_t     (std::uint64_t)

src_count_t          (std::size_t)
src_count8_t         (std::uint8_t)
src_count16_t        (std::uint16_t)
src_count32_t        (std::uint32_t)
src_count64_t        (std::uint64_t)

src_offset_count_t    (std::size_t)
src_offset_count8_t   (std::uint8_t)
src_offset_count16_t  (std::uint16_t)
src_offset_count32_t  (std::uint32_t)
src_offset_count64_t  (std::uint64_t)

dst_count_t          (std::size_t)
dst_count8_t         (std::uint8_t)
dst_count16_t        (std::uint16_t)
dst_count32_t        (std::uint32_t)
dst_count64_t        (std::uint64_t)

dst_offset_count_t    (std::size_t)
dst_offset_count8_t   (std::uint8_t)
dst_offset_count16_t  (std::uint16_t)
dst_offset_count32_t  (std::uint32_t)
dst_offset_count64_t  (std::uint64_t)

index_t              (std::size_t)
index8_t             (std::uint8_t)
index16_t            (std::uint16_t)
index32_t            (std::uint32_t)
index64_t            (std::uint64_t)

offset_t             (std::size_t)
offset8_t            (std::uint8_t)
offset16_t           (std::uint16_t)
offset32_t           (std::uint32_t)
offset64_t           (std::uint64_t)

src_offset_t         (std::size_t)
src_offset8_t        (std::uint8_t)
src_offset16_t       (std::uint16_t)
src_offset32_t       (std::uint32_t)
src_offset64_t       (std::uint64_t)

dst_offset_t         (std::size_t)
dst_offset8_t        (std::uint8_t)
dst_offset16_t       (std::uint16_t)
dst_offset32_t       (std::uint32_t)
dst_offset64_t       (std::uint64_t)
```

## TODO

- Maybe implement support for older versions of C++.
- Verify cross-platform support.
- Implement support for [C++20 modules](https://en.cppreference.com/w/cpp/language/modules).
- ???
