#pragma once

/*
	macros.hpp description:
		This header file contains various macros used by the strict_types library.
		All non-type-defining macros will be defined in this header file.

		Defined macros:
			STRICT_TYPES_NAMESPACE
			STRICT_TYPES_DEFINE_FORMATTER
			STRICT_TYPES_DEFINE_UNARY_INCR_DECR_OPERATORS
			STRICT_TYPES_DEFINE_ARITHMETIC_OPERATORS
			STRICT_TYPES_DEFINE_COMPARISON_OPERATORS
*/

// STRICT_TYPES_NAMESPACE should already be defined by the build system.
// Otherwise it will be defined here automatically with the default
// 'strict' name.
#if !defined(STRICT_TYPES_NAMESPACE)
	#define STRICT_TYPES_NAMESPACE strict
#endif

//	Defines a formatter for a strict_types integral or strict_types floating-point type.
//	Used primarily by other strict_types macros.
#define STRICT_TYPES_DEFINE_FORMATTER(TYPE)                                                                                                                                                             \
	template <>                                                                                                                                                                                          \
	struct _STD formatter<STRICT_TYPES_NAMESPACE::TYPE> {                                                                                                                                                \
			[[nodiscard]] inline constexpr auto parse(const _STD format_parse_context& context) const noexcept { return context.begin(); }                                                                 \
			[[nodiscard]] inline constexpr auto parse(const _STD wformat_parse_context& context) const noexcept { return context.begin(); }                                                                \
			[[nodiscard]] inline auto				format(const STRICT_TYPES_NAMESPACE::TYPE value, _STD format_context& context) const { return _STD format_to(context.out(), "{}", value.to_string()); }    \
			[[nodiscard]] inline auto				format(const STRICT_TYPES_NAMESPACE::TYPE value, _STD wformat_context& context) const { return _STD format_to(context.out(), L"{}", value.to_wstring()); } \
	};

// Defines unary and incremental/decremental operators.
// Normally only used inside the integral.hpp or float.hpp header files.
//
// Usage example:
//    STRICT_TYPES_DEFINE_UNARY_INCR_DECR_OPERATORS(integral)
//    STRICT_TYPES_DEFINE_UNARY_INCR_DECR_OPERATORS(float)
#define STRICT_TYPES_DEFINE_UNARY_INCR_DECR_OPERATORS(STRICT_TYPE_CONCEPT)                                           \
	template <typename Type>                                                                                          \
		requires STRICT_TYPES_NAMESPACE::detail::is_qualified_                                                         \
	##STRICT_TYPE_CONCEPT##_operator<Type> inline constexpr Type operator++(Type& value) noexcept {                   \
		return static_cast<Type>(++value.value);                                                                       \
	}                                                                                                                 \
                                                                                                                     \
	template <typename Type>                                                                                          \
		requires STRICT_TYPES_NAMESPACE::detail::is_qualified_                                                         \
	##STRICT_TYPE_CONCEPT##_operator<Type> inline constexpr Type operator++(Type& value, auto) noexcept {             \
		return static_cast<Type>(value.value++);                                                                       \
	}                                                                                                                 \
                                                                                                                     \
	template <typename Type>                                                                                          \
		requires STRICT_TYPES_NAMESPACE::detail::is_qualified_                                                         \
	##STRICT_TYPE_CONCEPT##_operator<Type> inline constexpr Type operator--(Type& value) noexcept {                   \
		return static_cast<Type>(--value.value);                                                                       \
	}                                                                                                                 \
                                                                                                                     \
	template <typename Type>                                                                                          \
		requires STRICT_TYPES_NAMESPACE::detail::is_qualified_                                                         \
	##STRICT_TYPE_CONCEPT##_operator<Type> inline constexpr Type operator--(Type& value, auto) noexcept {             \
		return static_cast<Type>(value.value--);                                                                       \
	}                                                                                                                 \
                                                                                                                     \
	template <typename Type>                                                                                          \
		requires STRICT_TYPES_NAMESPACE::detail::is_qualified_                                                         \
	##STRICT_TYPE_CONCEPT##_operator<Type> [[nodiscard]] inline constexpr Type operator+(const Type value) noexcept { \
		return static_cast<Type>(+value.value);                                                                        \
	}                                                                                                                 \
                                                                                                                     \
	template <typename Type>                                                                                          \
		requires STRICT_TYPES_NAMESPACE::detail::is_qualified_                                                         \
	##STRICT_TYPE_CONCEPT##_operator<Type> [[nodiscard]] inline constexpr Type operator-(const Type value) noexcept { \
		return static_cast<Type>(-value.value);                                                                        \
	}

// This macro defines a tri-collection of arithmetic operators specified by [OP].
// Normally only used inside the integral.hpp or float.hpp header files.
//
// Usage example:
//    STRICT_TYPES_DEFINE_ARITHMETIC_OPERATORS(+, float)
//    STRICT_TYPES_DEFINE_ARITHMETIC_OPERATORS(/, float)
//    STRICT_TYPES_DEFINE_ARITHMETIC_OPERATORS(*, integral)
#define STRICT_TYPES_DEFINE_ARITHMETIC_OPERATORS(OP, STRICT_TYPE_CONCEPT)                                                                                  \
	template <typename Left, typename Right>                                                                                                                \
		requires STRICT_TYPES_NAMESPACE::detail::is_qualified_                                                                                               \
	##STRICT_TYPE_CONCEPT##_operator<Left, Right> [[nodiscard]] inline constexpr Left operator OP(const Left left, const Right right) noexcept {            \
		return static_cast<Left>(left.value OP right.value);                                                                                                 \
	}                                                                                                                                                       \
	template <typename Left, typename Right>                                                                                                                \
		requires STRICT_TYPES_NAMESPACE::detail::is_qualified_                                                                                               \
	##STRICT_TYPE_CONCEPT##_operator_left_only<Left, Right> [[nodiscard]] inline constexpr Left operator OP(const Left left, const Right right) noexcept {  \
		return static_cast<Left>(left.value OP right);                                                                                                       \
	}                                                                                                                                                       \
	template <typename Left, typename Right>                                                                                                                \
		requires STRICT_TYPES_NAMESPACE::detail::is_qualified_                                                                                               \
	##STRICT_TYPE_CONCEPT##_operator_right_only<Left, Right> [[nodiscard]] inline constexpr Left operator OP(const Left left, const Right right) noexcept { \
		return static_cast<Left>(left OP right.value);                                                                                                       \
	}                                                                                                                                                       \
	template <typename Left, typename Right>                                                                                                                \
		requires STRICT_TYPES_NAMESPACE::detail::is_qualified_                                                                                               \
	##STRICT_TYPE_CONCEPT##_operator<Left, Right> inline constexpr Left& operator OP## = (Left & left, const Right right) noexcept {                        \
		left.value OP## = right.value;                                                                                                                       \
		return left;                                                                                                                                         \
	}                                                                                                                                                       \
	template <typename Left, typename Right>                                                                                                                \
		requires STRICT_TYPES_NAMESPACE::detail::is_qualified_                                                                                               \
	##STRICT_TYPE_CONCEPT##_operator_left_only<Left, Right> inline constexpr Left& operator OP## = (Left & left, const Right right) noexcept {              \
		left.value OP## = right;                                                                                                                             \
		return left;                                                                                                                                         \
	}                                                                                                                                                       \
	template <typename Left, typename Right>                                                                                                                \
		requires STRICT_TYPES_NAMESPACE::detail::is_qualified_                                                                                               \
	##STRICT_TYPE_CONCEPT##_operator_right_only<Left, Right> inline constexpr Left& operator OP## = (Left & left, const Right right) noexcept {             \
		left OP## = right.value;                                                                                                                             \
		return left;                                                                                                                                         \
	}

// Similar to STRICT_TYPES_DEFINE_ARITHMETIC_OPERATORS, this macro defines
// a tri-collection of comparison operators specified by [OP].
// Normally only used inside the integral.hpp or float.hpp header files.
//
// Usage example:
//    STRICT_TYPES_DEFINE_COMPARISON_OPERATORS(>, integral)
//    STRICT_TYPES_DEFINE_COMPARISON_OPERATORS(==, float)
//    STRICT_TYPES_DEFINE_COMPARISON_OPERATORS(!=, float)
#define STRICT_TYPES_DEFINE_COMPARISON_OPERATORS(OP, STRICT_TYPE_CONCEPT)                                                                                  \
	template <typename Left, typename Right>                                                                                                                \
		requires STRICT_TYPES_NAMESPACE::detail::is_qualified_                                                                                               \
	##STRICT_TYPE_CONCEPT##_operator<Left, Right> [[nodiscard]] inline constexpr bool operator OP(const Left left, const Right right) noexcept {            \
		return left.value OP right.value;                                                                                                                    \
	}                                                                                                                                                       \
	template <typename Left, typename Right>                                                                                                                \
		requires STRICT_TYPES_NAMESPACE::detail::is_qualified_                                                                                               \
	##STRICT_TYPE_CONCEPT##_operator_left_only<Left, Right> [[nodiscard]] inline constexpr bool operator OP(const Left left, const Right right) noexcept {  \
		return left.value OP right;                                                                                                                          \
	}                                                                                                                                                       \
	template <typename Left, typename Right>                                                                                                                \
		requires STRICT_TYPES_NAMESPACE::detail::is_qualified_                                                                                               \
	##STRICT_TYPE_CONCEPT##_operator_right_only<Left, Right> [[nodiscard]] inline constexpr bool operator OP(const Left left, const Right right) noexcept { \
		return left OP right.value;                                                                                                                          \
	}
