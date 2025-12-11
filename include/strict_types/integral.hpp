#pragma once

#include "macros.hpp"
#include "strict_types/concepts.hpp"
#include "strict_types/macros.hpp"

#include <cstdint>
#include <format>
#include <limits>
#include <string>
#include <type_traits>

#pragma push_macro("min")
#pragma push_macro("max")

#undef min
#undef max

/*
	integral.hpp description:
		This header file contains a strict_types integral-only encapsulation type, operators,
		and various pre-defined common integral types such as: int_t, size_t, long_t, etc.
		The encapsulated type must be an integral type.
*/

namespace STRICT_CPP_NAMESPACE {
	/// @brief Strictly-typed integral-only encapsulation struct.
	/// @tparam Type The encapsulated type.
	/// @tparam QualifiedTypes A range of qualified types suitable for implicit construction.
	template <typename Type, typename... QualifiedTypes>
		requires STRICT_CPP_NAMESPACE::detail::is_qualified_integral_type<Type, QualifiedTypes...>
	struct strict_integral_type : STRICT_CPP_NAMESPACE::detail::strict_cpp_integral_base_t {
			inline constexpr static Type min = std::numeric_limits<Type>::min();
			inline constexpr static Type max = std::numeric_limits<Type>::max();

			using type = Type;
			Type value = {};

			/// @brief Default constructor.
			inline constexpr strict_integral_type() noexcept = default;

			/// @brief Implicit copy constructor.
			/// @tparam Other The implicitly-convertible type.
			/// @param other The implicitly-convertible value.
			template <typename Other>
				requires STRICT_CPP_NAMESPACE::detail::is_qualified_implicit_constructor<Other, Type, QualifiedTypes...>
			inline constexpr strict_integral_type(const Other other) noexcept :
				value(static_cast<Type>(other)) { }

			/// @brief Explicit copy constructor.
			/// @tparam Other The explicitly-convertible type.
			/// @param other The explicitly-convertible value.
			template <typename Other>
				requires STRICT_CPP_NAMESPACE::detail::is_qualified_explicit_constructor<Other, Type>
			inline constexpr explicit strict_integral_type(const Other other) noexcept :
				value(static_cast<Type>(other)) { }

			/// @brief Copy assignment operator.
			/// @tparam Other The assignment type.
			/// @returns strict_integral_type&
			template <typename Other>
				requires STRICT_CPP_NAMESPACE::detail::is_qualified_integral_assignment_operator<Other>
			inline constexpr strict_integral_type& operator=(const Other other) noexcept {
				if constexpr (std::is_integral_v<Other>) this->value = static_cast<Type>(other);
				else this->value = static_cast<Type>(other.value);
				return *this;
			}

			/// @brief Implicit conversion operator.
			/// @returns Type&
			inline constexpr operator Type&() noexcept { return this->value; }

			/// @brief Implicit const conversion operator.
			/// @returns const Type&
			inline constexpr operator const Type&() const noexcept { return this->value; }

			/// @brief Explicit conversion operator.
			/// @tparam Other The type to convert to.
			/// @returns Other
			template <typename Other>
				requires STRICT_CPP_NAMESPACE::detail::is_qualified_explicit_conversion_operator<Type, Other>
			inline constexpr explicit operator Other() noexcept {
				return static_cast<Other>(this->value);
			}

			/// @brief Explicit const conversion operator.
			/// @tparam Other The type to convert to.
			/// @returns Other
			template <typename Other>
				requires STRICT_CPP_NAMESPACE::detail::is_qualified_explicit_conversion_operator<Type, Other>
			inline constexpr explicit operator const Other() const noexcept {
				return static_cast<const Other>(this->value);
			}

			/// @brief Conversion function.
			/// @tparam Other The type to convert to.
			/// @returns Other
			template <typename Other>
				requires STRICT_CPP_NAMESPACE::detail::is_qualified_conversion_fuction<Type, Other>
			inline constexpr Other as() noexcept {
				return static_cast<Other>(this->value);
			}

			/// @brief Const conversion function.
			/// @tparam Other The type to convert to.
			/// @returns const Other
			template <typename Other>
				requires STRICT_CPP_NAMESPACE::detail::is_qualified_conversion_fuction<Type, Other>
			inline constexpr const Other as() const noexcept {
				return static_cast<const Other>(this->value);
			}

			/// @brief Converts to a human-readable string representing the current value.
			/// @returns std::string
			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::can_convert_to_string_function<Type>
			inline std::string to_string() const { return std::to_string(this->value); }

			/// @brief Converts to a human-readable wide string representing the current value.
			/// @returns std::wstring
			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::can_convert_to_wstring_function<Type>
			inline std::wstring to_wstring() const { return std::to_wstring(this->value); }
	};

	// ==========================================================================
	// Operators
	// ==========================================================================

	STRICT_CPP_DEFINE_UNARY_INCR_DECR_OPERATORS(integral)

	STRICT_CPP_DEFINE_ARITHMETIC_OPERATORS(+, integral)
	STRICT_CPP_DEFINE_ARITHMETIC_OPERATORS(-, integral)
	STRICT_CPP_DEFINE_ARITHMETIC_OPERATORS(*, integral)
	STRICT_CPP_DEFINE_ARITHMETIC_OPERATORS(/, integral)
	STRICT_CPP_DEFINE_ARITHMETIC_OPERATORS(&, integral)
	STRICT_CPP_DEFINE_ARITHMETIC_OPERATORS(%, integral)
	STRICT_CPP_DEFINE_ARITHMETIC_OPERATORS(|, integral)
	STRICT_CPP_DEFINE_ARITHMETIC_OPERATORS(^, integral)
	STRICT_CPP_DEFINE_ARITHMETIC_OPERATORS(>>, integral)
	STRICT_CPP_DEFINE_ARITHMETIC_OPERATORS(<<, integral)

	STRICT_CPP_DEFINE_COMPARISON_OPERATORS(==, integral)
	STRICT_CPP_DEFINE_COMPARISON_OPERATORS(!=, integral)
	STRICT_CPP_DEFINE_COMPARISON_OPERATORS(>, integral)
	STRICT_CPP_DEFINE_COMPARISON_OPERATORS(<, integral)
	STRICT_CPP_DEFINE_COMPARISON_OPERATORS(>=, integral)
	STRICT_CPP_DEFINE_COMPARISON_OPERATORS(<=, integral)
	STRICT_CPP_DEFINE_COMPARISON_OPERATORS(<=>, integral)
}

// =============================================================================
// Macros
// =============================================================================

//	Defines an strict_types integral-only type.
#define STRICT_CPP_DEFINE_INTEGRAL_TYPE(NAME, TYPE, QUALIFIED_TYPES...)                                    \
	namespace STRICT_CPP_NAMESPACE {                                                                        \
		struct NAME : STRICT_CPP_NAMESPACE::strict_integral_type<TYPE, QUALIFIED_TYPES> {                    \
				using STRICT_CPP_NAMESPACE::strict_integral_type<TYPE, QUALIFIED_TYPES>::strict_integral_type; \
				using STRICT_CPP_NAMESPACE::strict_integral_type<TYPE, QUALIFIED_TYPES>::operator=;            \
		};                                                                                                   \
	}                                                                                                       \
	STRICT_CPP_DEFINE_FORMATTER(NAME)

#if defined(__cpp_char8_t)
	//	Defines a strict_types dynamic integral-only type.
	#define STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(NAME, QUALIFIED_TYPES...)                               \
		namespace STRICT_CPP_NAMESPACE {                                                                     \
			template <typename T>                                                                             \
				requires STRICT_CPP_NAMESPACE::detail::is_qualified_integral_type<QUALIFIED_TYPES>             \
			struct NAME : STRICT_CPP_NAMESPACE::strict_integral_type<T, QUALIFIED_TYPES> {                    \
					using STRICT_CPP_NAMESPACE::strict_integral_type<T, QUALIFIED_TYPES>::strict_integral_type; \
					using STRICT_CPP_NAMESPACE::strict_integral_type<T, QUALIFIED_TYPES>::operator=;            \
			};                                                                                                \
		}                                                                                                    \
		STRICT_CPP_DEFINE_FORMATTER(NAME<char>)                                                              \
		STRICT_CPP_DEFINE_FORMATTER(NAME<unsigned char>)                                                     \
		STRICT_CPP_DEFINE_FORMATTER(NAME<signed char>)                                                       \
		STRICT_CPP_DEFINE_FORMATTER(NAME<char8_t>)                                                           \
		STRICT_CPP_DEFINE_FORMATTER(NAME<char16_t>)                                                          \
		STRICT_CPP_DEFINE_FORMATTER(NAME<char32_t>)                                                          \
		STRICT_CPP_DEFINE_FORMATTER(NAME<short>)                                                             \
		STRICT_CPP_DEFINE_FORMATTER(NAME<unsigned short>)                                                    \
		STRICT_CPP_DEFINE_FORMATTER(NAME<int>)                                                               \
		STRICT_CPP_DEFINE_FORMATTER(NAME<unsigned int>)                                                      \
		STRICT_CPP_DEFINE_FORMATTER(NAME<long>)                                                              \
		STRICT_CPP_DEFINE_FORMATTER(NAME<unsigned long>)                                                     \
		STRICT_CPP_DEFINE_FORMATTER(NAME<long long>)                                                         \
		STRICT_CPP_DEFINE_FORMATTER(NAME<unsigned long long>)
#else
	//	Defines a strict_types dynamic integral-only type.
	#define STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(NAME, QUALIFIED_TYPES...)                               \
		namespace STRICT_CPP_NAMESPACE {                                                                     \
			template <typename T>                                                                             \
				requires STRICT_CPP_NAMESPACE::detail::is_qualified_integral_type<QUALIFIED_TYPES>             \
			struct NAME : STRICT_CPP_NAMESPACE::strict_integral_type<T, QUALIFIED_TYPES> {                    \
					using STRICT_CPP_NAMESPACE::strict_integral_type<T, QUALIFIED_TYPES>::strict_integral_type; \
					using STRICT_CPP_NAMESPACE::strict_integral_type<T, QUALIFIED_TYPES>::operator=;            \
			};                                                                                                \
		}                                                                                                    \
		STRICT_CPP_DEFINE_FORMATTER(NAME<char>)                                                              \
		STRICT_CPP_DEFINE_FORMATTER(NAME<unsigned char>)                                                     \
		STRICT_CPP_DEFINE_FORMATTER(NAME<signed char>)                                                       \
		STRICT_CPP_DEFINE_FORMATTER(NAME<char16_t>)                                                          \
		STRICT_CPP_DEFINE_FORMATTER(NAME<char32_t>)                                                          \
		STRICT_CPP_DEFINE_FORMATTER(NAME<short>)                                                             \
		STRICT_CPP_DEFINE_FORMATTER(NAME<unsigned short>)                                                    \
		STRICT_CPP_DEFINE_FORMATTER(NAME<int>)                                                               \
		STRICT_CPP_DEFINE_FORMATTER(NAME<unsigned int>)                                                      \
		STRICT_CPP_DEFINE_FORMATTER(NAME<long>)                                                              \
		STRICT_CPP_DEFINE_FORMATTER(NAME<unsigned long>)                                                     \
		STRICT_CPP_DEFINE_FORMATTER(NAME<long long>)                                                         \
		STRICT_CPP_DEFINE_FORMATTER(NAME<unsigned long long>)
#endif
// =============================================================================
// Pre-defined common types
// =============================================================================

STRICT_CPP_DEFINE_INTEGRAL_TYPE(char_t, char);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(_wchar_t, wchar_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(schar_t, signed char);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uchar_t, unsigned char);
#if defined(__cpp_char8_t)
STRICT_CPP_DEFINE_INTEGRAL_TYPE(_char8_t, char8_t);
#endif
STRICT_CPP_DEFINE_INTEGRAL_TYPE(_char16_t, char16_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(_char32_t, char32_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(short_t, short);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(ushort_t, unsigned short);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int_t, int);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint_t, unsigned int);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(long_t, long);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(ulong_t, unsigned long);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(long_long_t, long long);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(ulong_long_t, unsigned long long);

// size

STRICT_CPP_DEFINE_INTEGRAL_TYPE(size_t, std::size_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(size8_t, std::uint8_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(size16_t, std::uint16_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(size32_t, std::uint32_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(size64_t, std::uint64_t);

// int

STRICT_CPP_DEFINE_INTEGRAL_TYPE(int8_t, std::int8_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int16_t, std::int16_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int32_t, std::int32_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int64_t, std::int64_t);

STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint8_t, std::uint8_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint16_t, std::uint16_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint32_t, std::uint32_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint64_t, std::uint64_t);

// least

STRICT_CPP_DEFINE_INTEGRAL_TYPE(int_least8_t, std::int_least8_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int_least16_t, std::int_least16_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int_least32_t, std::int_least32_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int_least64_t, std::int_least64_t);

STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint_least8_t, std::uint_least8_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint_least16_t, std::uint_least16_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint_least32_t, std::uint_least32_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint_least64_t, std::uint_least64_t);

// fast

STRICT_CPP_DEFINE_INTEGRAL_TYPE(int_fast8_t, std::int_fast8_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int_fast16_t, std::int_fast16_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int_fast32_t, std::int_fast32_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int_fast64_t, std::int_fast64_t);

STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint_fast8_t, std::uint_fast8_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint_fast16_t, std::uint_fast16_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint_fast32_t, std::uint_fast32_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint_fast64_t, std::uint_fast64_t);

// intmax

STRICT_CPP_DEFINE_INTEGRAL_TYPE(intmax_t, std::intmax_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uintmax_t, std::uintmax_t);

// intptr

STRICT_CPP_DEFINE_INTEGRAL_TYPE(intptr_t, std::intptr_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uintptr_t, std::uintptr_t);

// byte

STRICT_CPP_DEFINE_INTEGRAL_TYPE(byte_t, std::uint8_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(sbyte_t, std::int8_t);

// =============================================================================
// Dynamic "any" integral types
// =============================================================================

// integers

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_integer_t,
	std::int8_t,
	std::int16_t,
	std::int32_t,
	std::int64_t,
	std::uint8_t,
	std::uint16_t,
	std::uint32_t,
	std::uint64_t,
	STRICT_CPP_NAMESPACE::schar_t,
	STRICT_CPP_NAMESPACE::uchar_t,
	STRICT_CPP_NAMESPACE::short_t,
	STRICT_CPP_NAMESPACE::ushort_t,
	STRICT_CPP_NAMESPACE::int_t,
	STRICT_CPP_NAMESPACE::uint_t,
	STRICT_CPP_NAMESPACE::long_t,
	STRICT_CPP_NAMESPACE::long_long_t,
	STRICT_CPP_NAMESPACE::ulong_t,
	STRICT_CPP_NAMESPACE::ulong_long_t,
	STRICT_CPP_NAMESPACE::int8_t,
	STRICT_CPP_NAMESPACE::int16_t,
	STRICT_CPP_NAMESPACE::int32_t,
	STRICT_CPP_NAMESPACE::int64_t,
	STRICT_CPP_NAMESPACE::uint8_t,
	STRICT_CPP_NAMESPACE::uint16_t,
	STRICT_CPP_NAMESPACE::uint32_t,
	STRICT_CPP_NAMESPACE::uint64_t
);

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_signed_integer_t,
	std::int8_t,
	std::int16_t,
	std::int32_t,
	std::int64_t,
	STRICT_CPP_NAMESPACE::schar_t,
	STRICT_CPP_NAMESPACE::short_t,
	STRICT_CPP_NAMESPACE::int_t,
	STRICT_CPP_NAMESPACE::long_t,
	STRICT_CPP_NAMESPACE::long_long_t,
	STRICT_CPP_NAMESPACE::int8_t,
	STRICT_CPP_NAMESPACE::int16_t,
	STRICT_CPP_NAMESPACE::int32_t,
	STRICT_CPP_NAMESPACE::int64_t
);

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_unsigned_integer_t,
	std::uint8_t,
	std::uint16_t,
	std::uint32_t,
	std::uint64_t,
	STRICT_CPP_NAMESPACE::uchar_t,
	STRICT_CPP_NAMESPACE::ushort_t,
	STRICT_CPP_NAMESPACE::uint_t,
	STRICT_CPP_NAMESPACE::ulong_t,
	STRICT_CPP_NAMESPACE::uint8_t,
	STRICT_CPP_NAMESPACE::uint16_t,
	STRICT_CPP_NAMESPACE::uint32_t,
	STRICT_CPP_NAMESPACE::uint64_t
);

// size

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_size_t,
	std::size_t,
	STRICT_CPP_NAMESPACE::size_t,
	STRICT_CPP_NAMESPACE::size8_t,
	STRICT_CPP_NAMESPACE::size16_t,
	STRICT_CPP_NAMESPACE::size32_t,
	STRICT_CPP_NAMESPACE::size64_t
);

// int8

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_int8_t,
	std::int8_t,
	STRICT_CPP_NAMESPACE::schar_t,
	STRICT_CPP_NAMESPACE::int8_t
);

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_uint8_t,
	std::uint8_t,
	STRICT_CPP_NAMESPACE::uchar_t,
	STRICT_CPP_NAMESPACE::uint8_t
);

// int16

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_int16_t,
	std::int16_t,
	STRICT_CPP_NAMESPACE::short_t,
	STRICT_CPP_NAMESPACE::int16_t
);

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_uint16_t,
	std::uint16_t,
	STRICT_CPP_NAMESPACE::ushort_t,
	STRICT_CPP_NAMESPACE::uint16_t
);

// int32

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_int32_t,
	std::int32_t,
	STRICT_CPP_NAMESPACE::int_t,
	STRICT_CPP_NAMESPACE::int32_t
);

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_uint32_t,
	std::uint32_t,
	STRICT_CPP_NAMESPACE::uint_t,
	STRICT_CPP_NAMESPACE::uint32_t
);

// int64

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_int64_t,
	std::int64_t,
	STRICT_CPP_NAMESPACE::long_t,
	STRICT_CPP_NAMESPACE::int64_t
);

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_uint64_t,
	std::uint64_t,
	STRICT_CPP_NAMESPACE::ulong_t,
	STRICT_CPP_NAMESPACE::uint64_t
);

// least

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_int_least_t,
	std::int_least8_t,
	std::int_least16_t,
	std::int_least32_t,
	std::int_least64_t,
	STRICT_CPP_NAMESPACE::int_least8_t,
	STRICT_CPP_NAMESPACE::int_least16_t,
	STRICT_CPP_NAMESPACE::int_least32_t,
	STRICT_CPP_NAMESPACE::int_least64_t
);

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_uint_least_t,
	std::uint_least8_t,
	std::uint_least16_t,
	std::uint_least32_t,
	std::uint_least64_t,
	STRICT_CPP_NAMESPACE::uint_least8_t,
	STRICT_CPP_NAMESPACE::uint_least16_t,
	STRICT_CPP_NAMESPACE::uint_least32_t,
	STRICT_CPP_NAMESPACE::uint_least64_t
);

// fast

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_int_fast_t,
	std::int_fast8_t,
	std::int_fast16_t,
	std::int_fast32_t,
	std::int_fast64_t,
	STRICT_CPP_NAMESPACE::int_fast8_t,
	STRICT_CPP_NAMESPACE::int_fast16_t,
	STRICT_CPP_NAMESPACE::int_fast32_t,
	STRICT_CPP_NAMESPACE::int_fast64_t
);

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_uint_fast_t,
	std::uint_fast8_t,
	std::uint_fast16_t,
	std::uint_fast32_t,
	std::uint_fast64_t,
	STRICT_CPP_NAMESPACE::uint_fast8_t,
	STRICT_CPP_NAMESPACE::uint_fast16_t,
	STRICT_CPP_NAMESPACE::uint_fast32_t,
	STRICT_CPP_NAMESPACE::uint_fast64_t
);

// intmax

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_intmax_t,
	std::uintmax_t,
	std::intmax_t,
	STRICT_CPP_NAMESPACE::intmax_t,
	STRICT_CPP_NAMESPACE::uintmax_t
);

// intptr

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_intptr_t,
	std::intptr_t,
	std::uintptr_t,
	STRICT_CPP_NAMESPACE::intptr_t,
	STRICT_CPP_NAMESPACE::uintptr_t
);

// byte

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_byte_t,
	std::int8_t,
	std::uint8_t,
	STRICT_CPP_NAMESPACE::byte_t,
	STRICT_CPP_NAMESPACE::sbyte_t
);

// Optional integral types:
#if defined(STRICT_CPP_OPTIONAL_TYPES)
	#define STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(NAME)         \
		STRICT_CPP_DEFINE_INTEGRAL_TYPE(NAME##_t, std::size_t);     \
		STRICT_CPP_DEFINE_INTEGRAL_TYPE(NAME##8_t, std::uint8_t);   \
		STRICT_CPP_DEFINE_INTEGRAL_TYPE(NAME##16_t, std::uint16_t); \
		STRICT_CPP_DEFINE_INTEGRAL_TYPE(NAME##32_t, std::uint32_t); \
		STRICT_CPP_DEFINE_INTEGRAL_TYPE(NAME##64_t, std::uint64_t); \
		STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(                    \
			any_##NAME##_t,                                          \
			std::uint8_t,                                            \
			std::uint16_t,                                           \
			std::uint32_t,                                           \
			std::uint64_t,                                           \
			STRICT_CPP_NAMESPACE::NAME##_t,                          \
			STRICT_CPP_NAMESPACE::NAME##8_t,                         \
			STRICT_CPP_NAMESPACE::NAME##16_t,                        \
			STRICT_CPP_NAMESPACE::NAME##32_t,                        \
			STRICT_CPP_NAMESPACE::NAME##64_t                         \
		);

// size_t

STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(begin_size)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(end_size)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(src_size)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(dst_size)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(lhs_size)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(rhs_size)

// offset_size_t

STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(offset_size)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(begin_offset_size)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(end_offset_size)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(src_offset_size)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(dst_offset_size)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(lhs_offset_size)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(rhs_offset_size)

// count_t

STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(count)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(begin_count)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(end_count)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(src_count)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(dst_count)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(lhs_count)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(rhs_count)

// offset_count_t

STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(offset_count)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(begin_offset_count)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(end_offset_count)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(src_offset_count)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(dst_offset_count)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(lhs_offset_count)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(rhs_offset_count)

// offset_t

STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(offset)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(begin_offset)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(end_offset)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(src_offset)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(dst_offset)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(lhs_offset)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(rhs_offset)

// index_t

STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(index)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(begin_index)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(end_index)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(src_index)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(dst_index)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(lhs_index)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(rhs_index)

// index_offset_t

STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(index_offset)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(begin_index_offset)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(end_index_offset)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(src_index_offset)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(dst_index_offset)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(lhs_index_offset)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(rhs_index_offset)

// capacity_t

STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(capacity)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(begin_capacity)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(end_capacity)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(src_capacity)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(dst_capacity)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(lhs_capacity)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(rhs_capacity)

// length_t

STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(length)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(begin_length)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(end_length)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(src_length)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(dst_length)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(lhs_length)
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(rhs_length)

	#undef STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-pragmas"

#pragma pop_macro("min")
#pragma pop_macro("max")

#pragma GCC diagnostic pop
