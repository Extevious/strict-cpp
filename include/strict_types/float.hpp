#pragma once

#include "strict_types/concepts.hpp"
#include "strict_types/macros.hpp"

#include <cmath>
#include <format>
#include <limits>
#include <string>
#include <type_traits>

#pragma push_macro("min")
#pragma push_macro("max")

#undef min
#undef max

/*
	float.hpp description:
		This header file contains a float-only strict encapsulation type, operators,
		and various pre-defined common float types such as: float_t, double_t, etc.
		The encapsulated type must be a floating-point type.
*/

namespace STRICT_TYPES_NAMESPACE {
	/// @brief Strictly-typed float-only encapsulation struct.
	/// @tparam Type The encapsulated type.
	/// @tparam QualifiedTypes A range of qualified types suitable for implicit construction.
	template <typename Type, typename... QualifiedTypes>
		requires STRICT_TYPES_NAMESPACE::detail::is_qualified_float_type<Type, QualifiedTypes...>
	struct strict_float_type : STRICT_TYPES_NAMESPACE::detail::strict_cpp_float_base_t {
			inline static constexpr Type min					  = std::numeric_limits<Type>::min();
			inline static constexpr Type max					  = std::numeric_limits<Type>::max();
			inline static constexpr Type lowest				  = std::numeric_limits<Type>::lowest();
			inline static constexpr Type quiet_NaN			  = std::numeric_limits<Type>::quiet_NaN();
			inline static constexpr Type signaling_NaN	  = std::numeric_limits<Type>::signaling_NaN();
			inline static constexpr Type positive_infinity = std::numeric_limits<Type>::infinity();
			inline static constexpr Type negative_infinity = -std::numeric_limits<Type>::infinity();
			inline static constexpr Type epsilon			  = std::numeric_limits<Type>::epsilon();
			inline static constexpr Type round_error		  = std::numeric_limits<Type>::round_error();
			inline static constexpr Type denorm_min		  = std::numeric_limits<Type>::denorm_min();

			using type = Type;
			Type value = {};

			/// @brief Default constructor.
			inline constexpr strict_float_type() noexcept = default;

			/// @brief Implicit copy constructor.
			/// @tparam Other The implicitly-convertible type.
			/// @param other The implicitly-convertible value.
			template <typename Other>
				requires STRICT_TYPES_NAMESPACE::detail::is_qualified_implicit_constructor<Other, Type, QualifiedTypes...>
			inline constexpr strict_float_type(const Other other) noexcept :
				value(static_cast<Type>(other)) { }

			/// @brief Explicit copy constructor.
			/// @tparam Other The explicitly-convertible type.
			/// @param other The explicitly-convertible value.
			template <typename Other>
				requires STRICT_TYPES_NAMESPACE::detail::is_qualified_explicit_constructor<Other, Type>
			inline constexpr explicit strict_float_type(const Other other) noexcept :
				value(static_cast<Type>(other)) { }

			/// @brief Implicit conversion operator.
			/// @returns Type&
			[[nodiscard]] inline constexpr operator Type&() noexcept { return this->value; }

			/// @brief Implicit const conversion operator.
			/// @returns const Type&
			[[nodiscard]] inline constexpr operator const Type&() const noexcept { return this->value; }

			/// @brief Explicit conversion operator.
			/// @tparam Other The type to convert to.
			/// @returns Other
			template <typename Other>
				requires STRICT_TYPES_NAMESPACE::detail::is_qualified_explicit_conversion_operator<Type, Other>
			[[nodiscard]] inline constexpr explicit operator Other() noexcept {
				return static_cast<Other>(this->value);
			}

			/// @brief Explicit const conversion operator.
			/// @tparam Other The type to convert to.
			/// @returns Other
			template <typename Other>
				requires STRICT_TYPES_NAMESPACE::detail::is_qualified_explicit_conversion_operator<Type, Other>
			[[nodiscard]] inline constexpr explicit operator const Other() const noexcept {
				return static_cast<const Other>(this->value);
			}

			/// @brief Conversion function.
			/// @tparam Other The type to convert to.
			/// @returns Other
			template <typename Other>
				requires STRICT_TYPES_NAMESPACE::detail::is_qualified_conversion_function<Type, Other>
			[[nodiscard]] inline constexpr Other as() noexcept {
				return static_cast<Other>(this->value);
			}

			/// @brief Const conversion function.
			/// @tparam Other The type to convert to.
			/// @returns const Other
			template <typename Other>
				requires STRICT_TYPES_NAMESPACE::detail::is_qualified_conversion_function<Type, Other>
			[[nodiscard]] inline constexpr const Other as() const noexcept {
				return static_cast<const Other>(this->value);
			}

			/// @brief Converts to a human-readable string representing the current value.
			/// @returns std::string
			template <typename _ = void>
				requires STRICT_TYPES_NAMESPACE::detail::can_convert_to_string_function<Type>
			[[nodiscard]] inline std::string to_string() const { return std::to_string(this->value); }

			/// @brief Converts to a human-readable wide string representing the current value.
			/// @returns std::wstring
			template <typename _ = void>
				requires STRICT_TYPES_NAMESPACE::detail::can_convert_to_wstring_function<Type>
			[[nodiscard]] inline std::wstring to_wstring() const { return std::to_wstring(this->value); }
	};

	// ==========================================================================
	// Operators
	// ==========================================================================

	/// @brief Float modulus operator returns the remainder between two strict floating-point types.
	/// @tparam Left Left-most strict floating-point type.
	/// @tparam Right Right-most strict floating-point type.
	/// @returns Left
	template <typename Left, typename Right>
		requires STRICT_TYPES_NAMESPACE::detail::is_qualified_float_operator<Left, Right>
	[[nodiscard]] inline static constexpr Left operator%(const Left left, const Right right) noexcept {
#if __cplusplus >= 202203L
		return static_cast<Left>(std::fmod(left.value, right.value));
#else
		if constexpr (std::is_same_v<typename Left::type, float> && std::is_same_v<typename Right::type, float>) return static_cast<Left>(std::fmodf(left.value, right.value));
		else return static_cast<Left>(std::fmodl(left.value, right.value));
#endif
	}

	/// @brief Float modulus operator returns the remainder between a strict floating-point type and a non-strict floating-point type.
	/// @tparam Left Left-most strict floating-point type.
	/// @tparam Right Right-most non-strict floating-point type.
	/// @returns Left
	template <typename Left, typename Right>
		requires STRICT_TYPES_NAMESPACE::detail::is_qualified_float_operator_left_only<Left, Right>
	[[nodiscard]] inline static constexpr Left operator%(const Left left, const Right right) noexcept {
#if __cplusplus >= 202203L
		return static_cast<Left>(std::fmod(left.value, right));
#else
		if constexpr (std::is_same_v<typename Left::type, float> && std::is_same_v<Right, float>) return static_cast<Left>(std::fmodf(left.value, right));
		else return static_cast<Left>(std::fmodl(left.value, right));
#endif
	}

	/// @brief Float modulus operator returns the remainder between a non-strict floating-point type and a strict floating-point type.
	/// @tparam Left Left-most non-strict floating-point type.
	/// @tparam Right Right-most strict floating-point type.
	/// @returns Left
	template <typename Left, typename Right>
		requires STRICT_TYPES_NAMESPACE::detail::is_qualified_float_operator_right_only<Left, Right>
	[[nodiscard]] inline static constexpr Left operator%(const Left left, const Right right) noexcept {
#if __cplusplus >= 202203L
		return static_cast<Left>(std::fmod(left, right.value));
#else
		if constexpr (std::is_same_v<Left, float> && std::is_same_v<typename Right::type, float>) return static_cast<Left>(std::fmodf(left, right.value));
		else return static_cast<Left>(std::fmodl(left, right.value));
#endif
	}

	/// @brief Float compound-assignment modulus operator assigns the remainder between two strict floating-point types.
	/// @tparam Left Left-most strict floating-point type.
	/// @tparam Right Right-most strict floating-point type.
	/// @returns Left&
	template <typename Left, typename Right>
		requires STRICT_TYPES_NAMESPACE::detail::is_qualified_float_operator<Left, Right>
	inline static constexpr Left& operator%=(Left& left, const Right right) noexcept {
#if __cplusplus >= 202203L
		left.value = static_cast<typename Left::type>(std::fmod(left.value, right.value));
#else
		if constexpr (std::is_same_v<typename Left::type, float> && std::is_same_v<typename Right::type, float>) left.value = static_cast<typename Left::type>(std::fmodf(left.value, right.value));
		else left.value = static_cast<typename Left::type>(std::fmodl(left.value, right.value));
#endif

		return left;
	}

	/// @brief Float compound-assignment modulus operator assigns the remainder between a strict floating-point type and a non-strict floating-point type.
	/// @tparam Left Left-most strict floating-point type.
	/// @tparam Right Right-most non-strict floating-point type.
	/// @returns Left&
	template <typename Left, typename Right>
		requires STRICT_TYPES_NAMESPACE::detail::is_qualified_float_operator_left_only<Left, Right>
	inline static constexpr Left& operator%=(Left& left, const Right right) noexcept {
#if __cplusplus >= 202203L
		left.value = static_cast<typename Left::type>(std::fmod(left.value, right));
#else
		if constexpr (std::is_same_v<typename Left::type, float> && std::is_same_v<Right, float>) left.value = static_cast<typename Left::type>(std::fmodf(left.value, right));
		else left.value = static_cast<typename Left::type>(std::fmodl(left.value, right));
#endif

		return left;
	}

	/// @brief Float compound-assignment modulus operator assigns the remainder between a non-strict floating-point type and a strict floating-point type.
	/// @tparam Left Left-most non-strict floating-point type.
	/// @tparam Right Right-most strict floating-point type.
	/// @returns Left&
	template <typename Left, typename Right>
		requires STRICT_TYPES_NAMESPACE::detail::is_qualified_float_operator_right_only<Left, Right>
	inline static constexpr Left& operator%=(Left& left, const Right right) noexcept {
#if __cplusplus >= 202203L
		left = static_cast<typename Left::type>(std::fmod(left, right.value));
#else
		if constexpr (std::is_same_v<typename Left::type, float> && std::is_same_v<typename Right::type, float>) left = static_cast<typename Left::type>(std::fmodf(left, right.value));
		else left = static_cast<typename Left::type>(std::fmodl(left, right.value));
#endif

		return left;
	}

	STRICT_TYPES_DEFINE_UNARY_INCR_DECR_OPERATORS(float)

	STRICT_TYPES_DEFINE_ARITHMETIC_OPERATORS(+, float)
	STRICT_TYPES_DEFINE_ARITHMETIC_OPERATORS(-, float)
	STRICT_TYPES_DEFINE_ARITHMETIC_OPERATORS(*, float)
	STRICT_TYPES_DEFINE_ARITHMETIC_OPERATORS(/, float)
}

// =============================================================================
// Macros
// =============================================================================

//	Defines a strict float-only type.
#define STRICT_TYPES_DEFINE_FLOAT_TYPE(NAME, TYPE, QUALIFIED_TYPES...)                                 \
	namespace STRICT_TYPES_NAMESPACE {                                                                  \
		struct NAME : STRICT_TYPES_NAMESPACE::strict_float_type<TYPE, QUALIFIED_TYPES> {                 \
				using STRICT_TYPES_NAMESPACE::strict_float_type<TYPE, QUALIFIED_TYPES>::strict_float_type; \
                                                                                                       \
				template <typename Other>                                                                  \
					requires STRICT_TYPES_NAMESPACE::detail::is_qualified_float_assignment_operator<Other>  \
				inline constexpr NAME& operator=(const Other other) noexcept {                             \
					if constexpr (std::is_floating_point_v<Other>) this->value = static_cast<TYPE>(other);  \
					else this->value = static_cast<TYPE>(other.value);                                      \
					return *this;                                                                           \
				}                                                                                          \
		};                                                                                               \
	}                                                                                                   \
	STRICT_TYPES_DEFINE_FORMATTER(NAME)

//	Defines a strict dynamic float-only type.
#define STRICT_TYPES_DEFINE_DYNAMIC_FLOAT_TYPE(NAME, QUALIFIED_TYPES...)                              \
	namespace STRICT_TYPES_NAMESPACE {                                                                 \
		template <typename T>                                                                           \
			requires STRICT_TYPES_NAMESPACE::detail::is_qualified_float_type<QUALIFIED_TYPES>            \
		struct NAME : STRICT_TYPES_NAMESPACE::strict_float_type<T, QUALIFIED_TYPES> {                   \
				using STRICT_TYPES_NAMESPACE::strict_float_type<T, QUALIFIED_TYPES>::strict_float_type;   \
                                                                                                      \
				template <typename Other>                                                                 \
					requires STRICT_TYPES_NAMESPACE::detail::is_qualified_float_assignment_operator<Other> \
				inline constexpr NAME& operator=(const Other other) noexcept {                            \
					if constexpr (std::is_floating_point_v<Other>) this->value = static_cast<T>(other);    \
					else this->value = static_cast<T>(other.value);                                        \
					return *this;                                                                          \
				}                                                                                         \
		};                                                                                              \
	}                                                                                                  \
	STRICT_TYPES_DEFINE_FORMATTER(NAME<float>)                                                         \
	STRICT_TYPES_DEFINE_FORMATTER(NAME<double>)                                                        \
	STRICT_TYPES_DEFINE_FORMATTER(NAME<long double>)

// =============================================================================
// Pre-defined common types
// =============================================================================

STRICT_TYPES_DEFINE_FLOAT_TYPE(float_t, float);
STRICT_TYPES_DEFINE_FLOAT_TYPE(double_t, double);
STRICT_TYPES_DEFINE_FLOAT_TYPE(long_double_t, long double);
STRICT_TYPES_DEFINE_FLOAT_TYPE(float32_t, std::float_t);
STRICT_TYPES_DEFINE_FLOAT_TYPE(float64_t, std::double_t);

// =============================================================================
// Pre-defined common dynamic types
// =============================================================================

STRICT_TYPES_DEFINE_DYNAMIC_FLOAT_TYPE(
	any_float_t,
	float,
	double,
	long double,
	STRICT_TYPES_NAMESPACE::float_t,
	STRICT_TYPES_NAMESPACE::double_t,
	STRICT_TYPES_NAMESPACE::long_double_t,
	STRICT_TYPES_NAMESPACE::float32_t,
	STRICT_TYPES_NAMESPACE::float64_t
);

STRICT_TYPES_DEFINE_DYNAMIC_FLOAT_TYPE(
	any_float32_t,
	float,
	STRICT_TYPES_NAMESPACE::float_t,
	STRICT_TYPES_NAMESPACE::float32_t
);

STRICT_TYPES_DEFINE_DYNAMIC_FLOAT_TYPE(
	any_float64_t,
	double,
	STRICT_TYPES_NAMESPACE::double_t,
	STRICT_TYPES_NAMESPACE::float64_t
);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-pragmas"

#pragma pop_macro("min")
#pragma pop_macro("max")

#pragma GCC diagnostic pop
