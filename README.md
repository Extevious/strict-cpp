## strict-cpp
Reduce ambiguity between methods and operators by using types provided by strict-cpp, a C++20 platform-agnostic project.

### Features:
 * Strict type wrappers encapsulating integral and floating-point types.
 * Singular header file.
 * [Meson Build](https://mesonbuild.com/) support.
 * MIT license.

### Can be used to...:
 * Reduce ambiguity between methods and operators.
 * Enforce more explicit type-safety.
 * Use method or operator overloads more freely with less ambiguity.

### Example:
```cpp
   // These could be ambiguous...
   std::int16_t add(std::int16_t a, std::int16_t b) { return a + b; } // #1
   std::int32_t add(std::int32_t a, std::int32_t b) { return a + b; } // #2

   // ...but these are not!
   strict::int16_t add(strict::int16_t a, strict::int16_t b) { return a + b; } // #3
   strict::int32_t add(strict::int32_t a, strict::int32_t b) { return a + b; } // #4

   ...

   // === If only the add() methods #1 and #2 existed ===
   //
   add(5, 6); // This would call add() #2 as expected.
   add(5U, 6); // This would also call add() #2 due to the 2nd argument qualifying.
   add(5U, 6U); // But this is ambiguous between #1 and #2.

   // === ...but if only the strict-type add() methods #3 and #4 existed ===
   //
   add(5, 6); // This would call add() #4 as expected.
   add(5U, 6); // This would fail due to no implicit conversion available for the first argument.
   add(5U, 6U);  // This would fail due to no implicit conversion available for both arguments.
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
   strict_cpp = strict_cpp_dependency
```

2. In your `meson.build` file, implement the following example:

```cmake
   # Your dependencies array.
   dependencies = []
   
   ...
   # the name of the subproject is the same as the filename.
   strict_cpp_subproject = subproject('strict-cpp')

   # strict_cpp_dependency is provided by strict-cpp in the meson.build file.
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
// Macro to define integral types (needs to be in the STRICT_CPP_NAMESPACE namespace).
//    STRICT_CPP_TYPE : your strict type's name.
//    T               : the underlying type (int, char, size_t, etc).
#define STRICT_CPP_DEFINE_INTEGRAL_TYPE(STRICT_CPP_TYPE, T)

// Macro to define floating-point types (needs to be in the STRICT_CPP_NAMESPACE namespace).
//    STRICT_CPP_TYPE : your strict type's name.
//    T               : the underlying type (float, double, etc).
#define STRICT_CPP_DEFINE_FLOAT_TYPE(STRICT_CPP_TYPE, T)  
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
* Add more types.
* Aligned types.
* Create types that are not arithmetic-based.
* Maybe implement support for older versions of C++.
* Tests.
* Verify cross-platform support.
* Implement support for [C++20 modules](https://en.cppreference.com/w/cpp/language/modules).
* ???