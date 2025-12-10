# strict_types

Use strictly-typed encapsulation types to reduce bugs, improve readability, and scale operator and function overloads.

```cpp
   // These could be ambiguous...
   add(short, short); // #1
   add(int, int);     // #2

   // ...but these are not!
   add(strict::int16_t, strict::int16_t); // #3
   add(strict::int32_t, strict::int32_t); // #4
```

### Features

- Strict types encapsulating integral, floating-point, and alias types.
- Explicit type-safety enforcement.
- Header-only files.
- Stringification and formatters.
- [Meson Build](https://mesonbuild.com/) support.
- MIT license.

### Usages

- Reduce ambiguity between methods and operators.
- Backwards-compatible with existing types.
- Reduce potential bugs like swapped parameters, undesired type conversions, etc.

### TODO

- Verify overhead impact.
- Verify compiled code has zero overhead in optimized builds.
- Implement debug assertion.
- Verify cross-platform support.
- Implement stream, hash, etc support.
- Implement bool, enum, and proxy types.
- Strict-ness levels.
- Implement support for [C++20 modules](https://en.cppreference.com/w/cpp/language/modules).
- CMake build file(s).
- ???

## Why use `strict_types`?

All strict types are explicit about which type is implicitly allowed to use its constructors, functions, and operators. This is why you can have a wide range of similar, but explicitly different types. I use my
strict types frequently to add various overloads, or reduce accidentally calling the wrong function. For example when allocating memory with the Vulkan API, I can do:

```cpp
   MemoryT allocateBufferMemory(std::size_t size);
   MemoryT allocateImageMemory(std::size_t size);
```

The above functions are *fine* for the most part, but you might accidentally
call `allocateBufferMemory` instead of `allocateImageMemory` by mistake. If the
parameter types were instead swapped out with their respective strict size type,
calling the wrong function would be technically impossible if using a strict type:

```cpp
   MemoryT allocateBufferMemory(strict::buffer_memory_size_t size);
   MemoryT allocateImageMemory(strict::image_memory_size_t size);
```

If a size value of type `strict::buffer_memory_size_t` was passed into the
`allocateImageMemory()` function, an error will be reported by your IDE, or
during compile time.

## Defining custom types

You can define your own custom types easily by using the below macros for integral, floats, and alias types. All strict integral and float types have "qualifier" types, which are defined after the second parameter in the macro.
A qualifier type allows for implicit usage of various non-strict or strict types during construction.

```cpp
   // NAME               : The name of your custom strict type.
   // TYPE               : The encapsulated type.
   // QUALIFIED_TYPES... : Various "qualifying" types.
   #define STRICT_CPP_DEFINE_INTEGRAL_TYPE(NAME, TYPE, QUALIFIED_TYPES...)

   // Examples:
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(my_size_t, std::size_t)
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(my_wildcard_size_t, std::size_t, int, unsigned int, std::uint64_t)
```

```cpp
   // NAME               : The name of your custom strict type.
   // TYPE               : The encapsulated type.
   // QUALIFIED_TYPES... : Various "qualifying" types.
   #define STRICT_CPP_DEFINE_FLOAT_TYPE(NAME, TYPE, QUALIFIED_TYPES...)

   // Examples:
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(my_float_t, float)
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(my_wildcard_float_t, float, double, long double)
```

Dynamic strict types are slightly different. Instead of an encapsulated type defined in the macro, it is declared in a template declaration. During development, you may want to easily swap out the encapsulated type through a template, using a dynamic strict type allows for just that.

```cpp
   // NAME               : The name of your custom strict type.
   // QUALIFIED_TYPES... : Various "qualifying" types.
   #define STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(NAME, QUALIFIED_TYPES...);
```

```cpp
   // NAME               : The name of your custom strict type.
   // QUALIFIED_TYPES... : Various "qualifying" types.
   #define STRICT_CPP_DEFINE_DYNAMIC_FLOAT_TYPE(NAME, QUALIFIED_TYPES...);
```

Strict alias types allow for encapsulating a non-reference type that isn't limited to a integral or floating-point type.

```cpp
   // NAME : The name of your custom strict type.
   // TYPE : The encapsulated type.
   #define STRICT_CPP_DEFINE_ALIAS_TYPE(NAME, TYPE);

   // Example:
   STRICT_CPP_DEFINE_ALIAS_TYPE(my_int_vector_t, std::vector<int>)
   STRICT_CPP_DEFINE_ALIAS_TYPE(my_name_string_t, std::string)
```

## How to include in your projects

### Cmake, Meson, or directly from GitHub

1. Clone or download this repo into your project.
2. Include the `strict-cpp.hpp` file that is inside the `include` directory.

### Meson (as a .wrap dependency)

1. Create a `strict-cpp.wrap` file in the `<project root>/subprojects` directory (if the directory doesn't exist, create one). An example `.wrap` file:

   ```python
      [wrap-git]
      url = https://github.com/Extevious/strict-cpp
      revision = head
      
      [provide]
      strict-cpp = strict_cpp_dependency
   ```

1. In your `meson.build` file, you should have something similar to following example:

   ```python
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

1. `strict-cpp.hpp` should be available once your project has been recreated.

## Macros

### Overridable

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

    any_int_t<T>           <user defined>
    any_size_t<T>          <user defined>
    any_signed_int_t<T>    <user defined>
    any_unsigned_int_t<T>  <user defined>
    any_int_least_t<T>     <user defined>
    any_int_fast_t<T>      <user defined>
    any_uint_least_t<T>    <user defined>
    any_uint_fast_t<T>     <user defined>
    any_intmax_t<T>        <user defined>
    any_intptr_t<T>        <user defined>
    any_byte_t<T>          <user defined>
```

```cpp
    float_t                float
    double_t               double
    long_double_t          long double
    float32_t              std::float_t
    float64_t              std::double_t

    any_float_t<T>         <user defined>
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
