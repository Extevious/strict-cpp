# strict_types

Use strictly-typed encapsulation types to reduce bugs, improve readability, and scale operator and function overloads.

```cpp
   // These could be ambiguous...
   add(short, short);
   add(int,   int);

   // ...but these are not!
   add(strict::int16_t, strict::int16_t);
   add(strict::int32_t, strict::int32_t);
```

## Features

- Explicit type-safety enforcement.
- Strict types encapsulating integral, floating-point, and custom types.
- Header-only files.
- Backwards-compatibility with existing types.
- Stringification and formatters.
- [Meson Build](https://mesonbuild.com/) support.
- MIT license.

### Usages

- Reduce ambiguity between methods and operators.
- Reduce potential bugs like swapped parameters, undesired type conversions, etc.

### TODO

- Verify compiled code has zero overhead in optimized builds.
- Implement debug assertion.
- Verify cross-platform support.
- Implement stream, hash, etc support.
- Implement bool, enum, and proxy types.
- Strict-ness levels.
- Implement support for [C++20 modules](https://en.cppreference.com/w/cpp/language/modules).
- CMake build file(s).
- ???\
  &nbsp;

## Why use `strict_types`?

All strict types are explicit about which type is implicitly allowed to be used in its constructors, functions, and operators. This is why you can have a wide range of similar, but explicitly different types. I use my strict types frequently to add various overloads, or reduce accidentally calling the wrong function. For example, when allocating memory I can do:

```cpp
   MemoryT allocate_buffer_memory(std::size_t size);
   MemoryT allocate_image_memory(std::size_t size);
   MemoryT allocate_text_memory(std::size_t size);
```

The above functions are _fine_ for the most part, but you might accidentally call `allocate_buffer_memory()` instead of `allocate_image_memory()` by mistake. If the parameter types were instead swapped out with their respective strict size type, calling the wrong function would be impossible when passing in the respective strict type.

```cpp
   MemoryT allocate_buffer_memory(strict::buffer_memory_size_t size);
   MemoryT allocate_image_memory(strict::image_memory_size_t size);
   MemoryT allocate_text_memory(strict::text_memory_size_t size);
```

If a size value of type `strict::buffer_memory_size_t` was passed into the `allocate_image_memory()` function, an error will be reported by your IDE, or during compile time. You can still, however, pass in a non-strict type with no errors reported so long as it's the same type as the encapsulated type. For more examples, check out [these examples](/examples).\
&nbsp;

## Installation/Setup

Since `strict_types` is made up of header-only C++ source files, you can download/clone and reference the files directly, or use the provided build system files.

### Meson Build System

| Options            | Type      | Default value | Description                                   |
| ------------------ | --------- | ------------- | --------------------------------------------- |
| namespace          | `string`  | `strict`      | The namespace for strict types.               |
| enable_extra_types | `boolean` | `false`       | If pre-defined extra types should be exposed. |
| build_examples     | `boolean` | `false`       | If examples should be built.                  |

### Including `strict_types` using a meson .wrap file

1. Create a `strict_types.wrap` file in the `<project root>/subprojects` directory (if the directory doesn't exist, create one). An example `.wrap` file:

   ```python
      [wrap-git]
      url = https://github.com/Extevious/strict_types
      revision = head

      [provide]
      strict_types = strict_types_dep
   ```

1. In your `meson.build` file, you should have something similar to following example:

   ```python
       # Your dependencies array.
       dependencies = []

       ...

       # The name of the subproject is the same as the strict_types.wrap filename.
       strict_types_subproject = subproject('strict_types')

       # You can use the dependency [provide] exposes in the strict_types.wrap file:
       dependencies += dependency('strict_types')
       #
       # Or...
       #
       # use the strict_types_dep that is provided by the strict_types meson.build file:
       dependencies += strict_types_subproject.get_variable('strict_types_dep')

       ...

       # Example executable.
       executable('MyProject', 'main.cpp', dependencies : strict_types_dep)
   ```

1. Recreate your build files. `strict_types` should be available once your project has been recreated.\
   &nbsp;

## Defining custom types

You can define your own custom types easily by using the `STRICT_TYPES_DEFINE_*` macros for integral, floating-point, and alias types.

**Strict dynamic types:** Dynamic strict types are slightly different than their non-dynamic equivalent. Instead of an encapsulated type defined in the macro, it is declared using templates. During development, you may want to easily swap out the encapsulated type through a template. Using a dynamic strict type allows for just that.

**Strict alias types:** Strict alias types allow for encapsulating a non-reference type that isn't limited to a integral or floating-point type. Strict alias types also has a dynamic variant, which behaves the same way as a dynamic integral or float type. The difference is you still need to specify the encapsulated type in the macro, but without the template brackets.

## Defining custom types: Macros

**Currently defined are the following macros:**

- `STRICT_TYPES_DEFINE_INTEGRAL_TYPE`
- `STRICT_TYPES_DEFINE_FLOAT_TYPE`
- `STRICT_TYPES_DEFINE_ALIAS_TYPE`
- `STRICT_TYPES_DEFINE_DYNAMIC_INTEGRAL_TYPE`
- `STRICT_TYPES_DEFINE_DYNAMIC_FLOAT_TYPE`
- `STRICT_TYPES_DEFINE_DYNAMIC_ALIAS_TYPE`

&nbsp;\
&nbsp;

> ### `STRICT_TYPES_DEFINE_INTEGRAL_TYPE(NAME, TYPE, QUALIFIED_TYPES...)`
>
> | Parameter Name  | Type       | Description                                                                                         |
> | --------------- | ---------- | --------------------------------------------------------------------------------------------------- |
> | NAME            | `raw text` | The name of your custom strict type. Conventionally ends with `_t` to denote it's a primitive type. |
> | TYPE            | `type`     | The encapsulated type. Must be an integral type.                                                    |
> | QUALIFIED_TYPES | `types...` | A range of integral types. These types enable implicit usage of their respective type.              |
>
> ```cpp
>   // Macro example:
>   STRICT_TYPES_DEFINE_INTEGRAL_TYPE(my_size_t, std::size_t, int, unsigned int, std::uint64_t)
>
>   // Usage example:
>   strict::my_size_t my_size = 5;
> ```

&nbsp;\
&nbsp;

> ### `STRICT_TYPES_DEFINE_FLOAT_TYPE(NAME, TYPE, QUALIFIED_TYPES...)`
>
> | Parameter Name  | Type       | Description                                                                                         |
> | --------------- | ---------- | --------------------------------------------------------------------------------------------------- |
> | NAME            | `raw text` | The name of your custom strict type. Conventionally ends with `_t` to denote it's a primitive type. |
> | TYPE            | `type`     | The encapsulated type. Must be a floating-point type.                                               |
> | QUALIFIED_TYPES | `types...` | A range of floating-point types. These types enable implicit usage of their respective type.        |
>
> ```cpp
>   // Macro example:
>   STRICT_TYPES_DEFINE_FLOAT_TYPE(my_float_t, float, double, long double)
>
>   // Usage example:
>   strict::my_float_t my_float = 5.0d;
> ```

&nbsp;\
&nbsp;

> ### `STRICT_TYPES_DEFINE_ALIAS_TYPE(NAME, TYPE)`
>
> | Parameter Name | Type       | Description                                           |
> | -------------- | ---------- | ----------------------------------------------------- |
> | NAME           | `raw text` | The name of your custom strict type.                  |
> | TYPE           | `type`     | The encapsulated type. Must be a non-zero sized type. |
>
> ```cpp
>   // Macro examples:
>   STRICT_TYPES_DEFINE_ALIAS_TYPE(my_int_vector,  std::vector<int>)
>   STRICT_TYPES_DEFINE_ALIAS_TYPE(my_name_string, std::string     )
>
>   // Usage examples:
>   strict::my_int_vector  my_vector = {1, 2, 3};
>   strict::my_name_string my_name   = "Joe Swanson";
> ```

&nbsp;\
&nbsp;

> ### `STRICT_TYPES_DEFINE_DYNAMIC_INTEGRAL_TYPE(NAME, TYPE)`
>
> | Parameter Name | Type       | Description                                                                                         |
> | -------------- | ---------- | --------------------------------------------------------------------------------------------------- |
> | NAME           | `raw text` | The name of your custom strict type. Conventionally ends with `_t` to denote it's a primitive type. |
> | TYPE           | `type`     | The encapsulated type. Must be a floating-point type.                                               |
>
> ```cpp
>   // Macro example:
>   STRICT_TYPES_DEFINE_DYNAMIC_INTEGRAL_TYPE(my_dynamic_int_t, int, std::uint64_t)
>
>   // Usage examples:
>   strict::my_dynamic_int_t<unsigned long int> my_int       = 5ULL;
>   strict::my_dynamic_int_t<int>               my_other_int = 32;
> ```

&nbsp;\
&nbsp;

> ### `STRICT_TYPES_DEFINE_DYNAMIC_FLOAT_TYPE(NAME, TYPE)`
>
> | Parameter Name | Type       | Description                                                                                         |
> | -------------- | ---------- | --------------------------------------------------------------------------------------------------- |
> | NAME           | `raw text` | The name of your custom strict type. Conventionally ends with `_t` to denote it's a primitive type. |
> | TYPE           | `type`     | The encapsulated type. Must be a floating-point type.                                               |
>
> ```cpp
>   // Macro examples:
>   STRICT_TYPES_DEFINE_DYNAMIC_FLOAT_TYPE(my_dynamic_float_t, float, double, long double)
>
>   // Usage examples:
>   strict::my_dynamic_float_t<float>  my_float       = 5.0f;
>   strict::my_dynamic_float_t<double> my_other_float = 3.14d;
> ```

&nbsp;\
&nbsp;

> ### `STRICT_TYPES_DEFINE_DYNAMIC_ALIAS_TYPE(NAME, TYPE)`
>
> | Parameter Name | Type       | Description                                           |
> | -------------- | ---------- | ----------------------------------------------------- |
> | NAME           | `raw text` | The name of your custom strict type.                  |
> | TYPE           | `type`     | The encapsulated type. Must be a non-zero sized type. |
>
> ```cpp
>   // Macro examples:
>   STRICT_TYPES_DEFINE_ALIAS_TYPE(my_vector, std::vector)
>   STRICT_TYPES_DEFINE_ALIAS_TYPE(my_array, std::array)
>
>   // Usage examples:
>   strict::my_vector<int>    my_vector = {1, 2, 3};
>   strict::my_array<bool, 2> my_array  = {true, false};
> ```

&nbsp;

## User-Definable Macros

> ### `STRICT_TYPES_NAMESPACE`
>
> **Description**\
> The name of the `strict_types` namespace. If the macro is left undefined, it will be automatically defined with the default value `strict`.
>
> | Requirements     |          |
> | ---------------- | -------- |
> | Default value    | `strict` |
> | Can be undefined | `true`   |
> | Value type       | `text`   |

### `STRICT_TYPES_ENABLE_EXTRA_TYPES`

**Description**\
When defined it enables pre-defined extra types. Some extra types included are: `offset_t` `count_t` `capacity_t` `index_t`

| Requirements     |           |
| ---------------- | --------- |
| Default value    | undefined |
| Can be undefined | `true`    |
| Value type       | `any`     |

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

**_Note:_** _optional types available by defining the_ `STRICT_TYPES_EXTRA_TYPES` _macro._

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
    ...
```
