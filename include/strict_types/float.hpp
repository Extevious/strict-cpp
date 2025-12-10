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

namespace STRICT_CPP_NAMESPACE {
	/// @brief Strictly-typed float-only encapsulation struct.
	/// @tparam Type The encapsulated type.
	/// @tparam QualifiedTypes A range of qualified types suitable for implicit construction.
	template <typename Type, typename... QualifiedTypes>
		requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_type<Type>
	struct strict_float_type : STRICT_CPP_NAMESPACE::detail::strict_cpp_float_base_t {
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
				requires STRICT_CPP_NAMESPACE::detail::is_qualified_implicit_constructor<Other, Type, QualifiedTypes...>
			inline constexpr strict_float_type(const Other other) noexcept :
				value(static_cast<Type>(other)) { }

			/// @brief Explicit copy constructor.
			/// @tparam Other The explicitly-convertible type.
			/// @param other The explicitly-convertible value.
			template <typename Other>
				requires STRICT_CPP_NAMESPACE::detail::is_qualified_explicit_constructor<Other, Type>
			inline constexpr explicit strict_float_type(const Other other) noexcept :
				value(static_cast<Type>(other)) { }

			/// @brief Assignment operator.
			/// @tparam Other The assignment type.
			/// @returns strict_float_type&
			template <typename Other>
				requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_assignment_operator<Other>
			inline constexpr strict_float_type& operator=(const Other other) noexcept {
				if constexpr (std::is_floating_point_v<Other>) this->value = other;
				else this->value = other.value;
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

	template <typename Left, typename Right>
		requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator<Left, Right>
	inline constexpr Left operator%(const Left left, const Right right) noexcept {
		if constexpr (__cplusplus >= 202207L) return static_cast<Left>(std::fmod(left.value, right.value));
		else return static_cast<Left>(std::fmodl(left.value, right.value));
	}

	template <typename Left, typename Right>
		requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator_left_only<Left, Right>
	inline constexpr Left operator%(const Left left, const Right right) noexcept {
		if constexpr (__cplusplus >= 202207L) return static_cast<Left>(std::fmod(left.value, right));
		else return static_cast<Left>(std::fmodl(left.value, right));
	}

	template <typename Left, typename Right>
		requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator_right_only<Left, Right>
	inline constexpr Left operator%(const Left left, const Right right) noexcept {
		if constexpr (__cplusplus >= 202207L) return static_cast<Left>(std::fmod(left, right.value));
		else return static_cast<Left>(std::fmodl(left, right.value));
	}

	template <typename Left, typename Right>
		requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator<Left, Right>
	inline constexpr Left& operator%=(Left& left, const Right right) noexcept {
		if constexpr (__cplusplus >= 202207L) left.value = static_cast<typename Left::type>(std::fmod(left.value, right.value));
		else left.value = static_cast<typename Left::type>(std::fmodl(left.value, right.value));

		return left;
	}

	template <typename Left, typename Right>
		requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator_left_only<Left, Right>
	inline constexpr Left& operator%=(Left& left, const Right right) noexcept {
		if constexpr (__cplusplus >= 202207L) left.value = static_cast<typename Left::type>(std::fmod(left.value, right));
		else left.value = static_cast<typename Left::type>(std::fmodl(left.value, right));

		return left;
	}

	template <typename Left, typename Right>
		requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator_right_only<Left, Right>
	inline constexpr Left& operator%=(Left& left, const Right right) noexcept {
		if constexpr (__cplusplus >= 202207L) left = static_cast<typename Left::type>(std::fmod(left, right.value));
		else left = static_cast<typename Left::type>(std::fmodl(left, right.value));

		return left;
	}

	DEFINE_STRICT_CPP_UNARY_INCR_DECR_OPERATORS(float)

	STRICT_CPP_DEFINE_ARITHMETIC_OPERATORS(+, float)
	STRICT_CPP_DEFINE_ARITHMETIC_OPERATORS(-, float)
	STRICT_CPP_DEFINE_ARITHMETIC_OPERATORS(*, float)
	STRICT_CPP_DEFINE_ARITHMETIC_OPERATORS(/, float)
}

// =============================================================================
// Macros
// =============================================================================

//	Defines a strict float-only type.
//
//	Usage example:
//	   STRICT_CPP_DEFINE_INTEGRAL_TYPE(float32_t, float)
#define STRICT_CPP_DEFINE_FLOAT_TYPE(NAME, TYPE, QUALIFIED_TYPES...)                                 \
	namespace STRICT_CPP_NAMESPACE {                                                                  \
		struct NAME : STRICT_CPP_NAMESPACE::strict_float_type<TYPE, QUALIFIED_TYPES> {                 \
				using STRICT_CPP_NAMESPACE::strict_float_type<TYPE, QUALIFIED_TYPES>::strict_float_type; \
				using STRICT_CPP_NAMESPACE::strict_float_type<TYPE, QUALIFIED_TYPES>::operator=;         \
		};                                                                                             \
	}                                                                                                 \
	STRICT_CPP_DEFINE_FORMATTER(NAME)

//	Defines a strict float-only dynamic type.
//
//	Usage example:
//		STRICT_CPP_DEFINE_DYNAMIC_FLOAT_TYPE(
//			any_float32_t,
//			float,
//			STRICT_CPP_NAMESPACE::float32_t
//		)
#define STRICT_CPP_DEFINE_DYNAMIC_FLOAT_TYPE(NAME, QUALIFIED_TYPES...)                            \
	namespace STRICT_CPP_NAMESPACE {                                                               \
		template <typename T>                                                                       \
			requires std::is_floating_point_v<T>                                                     \
		struct NAME : STRICT_CPP_NAMESPACE::strict_float_type<T, QUALIFIED_TYPES> {                 \
				using STRICT_CPP_NAMESPACE::strict_float_type<T, QUALIFIED_TYPES>::strict_float_type; \
				using STRICT_CPP_NAMESPACE::strict_float_type<T, QUALIFIED_TYPES>::operator=;         \
		};                                                                                          \
	}                                                                                              \
	STRICT_CPP_DEFINE_FORMATTER(NAME<float>)                                                       \
	STRICT_CPP_DEFINE_FORMATTER(NAME<double>)                                                      \
	STRICT_CPP_DEFINE_FORMATTER(NAME<long double>)

// =============================================================================
// Pre-defined types
// =============================================================================

// Common floating-point types:
STRICT_CPP_DEFINE_FLOAT_TYPE(float_t, float);
STRICT_CPP_DEFINE_FLOAT_TYPE(double_t, double);
STRICT_CPP_DEFINE_FLOAT_TYPE(long_double_t, long double);
STRICT_CPP_DEFINE_FLOAT_TYPE(float32_t, std::float_t);
STRICT_CPP_DEFINE_FLOAT_TYPE(float64_t, std::double_t);

// Dynamic "any" float types:
STRICT_CPP_DEFINE_DYNAMIC_FLOAT_TYPE(
	any_float_t,
	float,
	double,
	long double,
	STRICT_CPP_NAMESPACE::float_t,
	STRICT_CPP_NAMESPACE::double_t,
	STRICT_CPP_NAMESPACE::long_double_t,
	STRICT_CPP_NAMESPACE::float32_t,
	STRICT_CPP_NAMESPACE::float64_t
);

STRICT_CPP_DEFINE_DYNAMIC_FLOAT_TYPE(
	any_float32_t,
	float,
	STRICT_CPP_NAMESPACE::float_t,
	STRICT_CPP_NAMESPACE::float32_t
);

STRICT_CPP_DEFINE_DYNAMIC_FLOAT_TYPE(
	any_float64_t,
	double,
	STRICT_CPP_NAMESPACE::double_t,
	STRICT_CPP_NAMESPACE::float64_t
);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-pragmas"

#pragma pop_macro("min")
#pragma pop_macro("max")

#pragma GCC diagnostic pop
