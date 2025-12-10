#pragma once

#if !defined(STRICT_CPP_NAMESPACE)
	#define STRICT_CPP_NAMESPACE strict
#endif

#include <cmath>
#include <cstdint>
#include <format>
#include <limits>
#include <string>
#include <type_traits>

#pragma push_macro("min")
#pragma push_macro("max")

#undef min
#undef max

#define STRICT_CPP_DEFINE_INTEGRAL_OPERATOR(OPERATOR_TYPE)                                          \
	template <typename Left, typename Right>                                                         \
		requires STRICT_CPP_NAMESPACE::detail::is_qualified_integral_operator<Left, Right>            \
	inline constexpr Left operator OPERATOR_TYPE(const Left left, const Right right) noexcept {      \
		return static_cast<Left>(left.value OPERATOR_TYPE right.value);                               \
	}                                                                                                \
	template <typename Left, typename Right>                                                         \
		requires STRICT_CPP_NAMESPACE::detail::is_qualified_integral_operator_left_only<Left, Right>  \
	inline constexpr Left operator OPERATOR_TYPE(const Left left, const Right right) noexcept {      \
		return static_cast<Left>(left.value OPERATOR_TYPE right);                                     \
	}                                                                                                \
	template <typename Left, typename Right>                                                         \
		requires STRICT_CPP_NAMESPACE::detail::is_qualified_integral_operator_right_only<Left, Right> \
	inline constexpr Left operator OPERATOR_TYPE(const Left left, const Right right) noexcept {      \
		return static_cast<Left>(left OPERATOR_TYPE right.value);                                     \
	}

#define STRICT_CPP_DEFINE_FLOAT_OPERATOR(OPERATOR_TYPE)                                          \
	template <typename Left, typename Right>                                                      \
		requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator<Left, Right>            \
	inline constexpr Left operator OPERATOR_TYPE(const Left left, const Right right) noexcept {   \
		return static_cast<Left>(left.value OPERATOR_TYPE right.value);                            \
	}                                                                                             \
	template <typename Left, typename Right>                                                      \
		requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator_left_only<Left, Right>  \
	inline constexpr Left operator OPERATOR_TYPE(const Left left, const Right right) noexcept {   \
		return static_cast<Left>(left.value OPERATOR_TYPE right);                                  \
	}                                                                                             \
	template <typename Left, typename Right>                                                      \
		requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator_right_only<Left, Right> \
	inline constexpr Left operator OPERATOR_TYPE(const Left left, const Right right) noexcept {   \
		return static_cast<Left>(left OPERATOR_TYPE right.value);                                  \
	}

#define STRICT_CPP_DEFINE_COMPOUND_INTEGRAL_OPERATOR(OPERATOR_TYPE)                                 \
	template <typename Left, typename Right>                                                         \
		requires STRICT_CPP_NAMESPACE::detail::is_qualified_integral_operator<Left, Right>            \
	inline constexpr Left& operator OPERATOR_TYPE(Left& left, const Right right) noexcept {          \
		left.value OPERATOR_TYPE right.value;                                                         \
		return left;                                                                                  \
	}                                                                                                \
	template <typename Left, typename Right>                                                         \
		requires STRICT_CPP_NAMESPACE::detail::is_qualified_integral_operator_left_only<Left, Right>  \
	inline constexpr Left& operator OPERATOR_TYPE(Left& left, const Right right) noexcept {          \
		left.value OPERATOR_TYPE right;                                                               \
		return left;                                                                                  \
	}                                                                                                \
	template <typename Left, typename Right>                                                         \
		requires STRICT_CPP_NAMESPACE::detail::is_qualified_integral_operator_right_only<Left, Right> \
	inline constexpr Left& operator OPERATOR_TYPE(Left& left, const Right right) noexcept {          \
		left OPERATOR_TYPE right.value;                                                               \
		return left;                                                                                  \
	}

#define STRICT_CPP_DEFINE_COMPOUND_FLOAT_OPERATOR(OPERATOR_TYPE)                                 \
	template <typename Left, typename Right>                                                      \
		requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator<Left, Right>            \
	inline constexpr Left& operator OPERATOR_TYPE(Left& left, const Right right) noexcept {       \
		left.value OPERATOR_TYPE right.value;                                                      \
		return left;                                                                               \
	}                                                                                             \
	template <typename Left, typename Right>                                                      \
		requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator_left_only<Left, Right>  \
	inline constexpr Left& operator OPERATOR_TYPE(Left& left, const Right right) noexcept {       \
		left.value OPERATOR_TYPE right;                                                            \
		return left;                                                                               \
	}                                                                                             \
	template <typename Left, typename Right>                                                      \
		requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator_right_only<Left, Right> \
	inline constexpr Left& operator OPERATOR_TYPE(Left& left, const Right right) noexcept {       \
		left OPERATOR_TYPE right.value;                                                            \
		return left;                                                                               \
	}

#define STRICT_CPP_DEFINE_COMPARISON_OPERATOR(OPERATOR_TYPE)                                   \
	template <typename Left, typename Right>                                                    \
		requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator<Left, Right>                \
	inline constexpr bool operator OPERATOR_TYPE(const Left left, const Right right) noexcept { \
		return left.value OPERATOR_TYPE right.value;                                             \
	}                                                                                           \
	template <typename Left, typename Right>                                                    \
		requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator_left_only<Left, Right>      \
	inline constexpr bool operator OPERATOR_TYPE(const Left left, const Right right) noexcept { \
		return left.value OPERATOR_TYPE right;                                                   \
	}                                                                                           \
	template <typename Left, typename Right>                                                    \
		requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator_right_only<Left, Right>     \
	inline constexpr bool operator OPERATOR_TYPE(const Left left, const Right right) noexcept { \
		return left OPERATOR_TYPE right.value;                                                   \
	}

namespace STRICT_CPP_NAMESPACE {
	namespace detail {
		// Base type for integral or float type qualifications.
		struct strict_cpp_base_t { };

		// Base type for alias-only type qualifications.
		struct strict_cpp_alias_base_t { };

		// Base type for integral-only type qualifications.
		struct strict_cpp_integral_base_t : strict_cpp_base_t { };

		// Base type for float-only type qualifications.
		struct strict_cpp_float_base_t : strict_cpp_base_t { };

		// Returns true if [Other] is the same as one of the [QualifiedTypes], and if [QualifiedTypes] is a non-zero length.
		template <typename Other, typename... QualifiedTypes>
		concept is_qualified_type = (sizeof...(QualifiedTypes) > 0) && (std::is_same_v<Other, QualifiedTypes> || ...);

		// Returns true if [Type] is an integral type, and trivial.
		template <typename Type>
		concept is_qualified_integral_type = std::is_integral_v<Type> && std::is_trivial_v<Type>;

		// Returns true if [Type] is a floating-point type, and trivial.
		template <typename Type>
		concept is_qualified_float_type = std::is_floating_point_v<Type> && std::is_trivial_v<Type>;

		// Returns true if [Other] is the same as one of the [QualifiedTypes], and if [QualifiedTypes] is a non-zero length.
		template <typename Other, typename... QualifiedTypes>
		concept is_qualified_implicit_constructor = is_qualified_type<Other, QualifiedTypes...>;

		// Returns true if [Other] is convertible to [Type].
		template <typename Other, typename Type>
		concept is_qualified_explicit_constructor = (std::is_convertible_v<Other, Type> || std::is_base_of_v<STRICT_CPP_NAMESPACE::detail::strict_cpp_base_t, Other>) && !std::is_same_v<Other, Type>;

		// Returns true if [Type] is convertible to [Other], but is not the exact same type.
		template <typename Type, typename Other>
		concept is_qualified_explicit_conversion_operator = std::is_convertible_v<Type, Other> && !std::is_same_v<Type, Other>;

		// Returns true if all [Types] inherits from the [strict_cpp_base_t] type.
		template <typename... Types>
		concept is_qualified_operator = (std::is_base_of_v<STRICT_CPP_NAMESPACE::detail::strict_cpp_base_t, Types> && ...);

		// Returns true if [Left] inherits from the [strict_cpp_base_t] type and Right is a scalar type.
		template <typename Left, typename Right>
		concept is_qualified_operator_left_only = is_qualified_operator<Left> && std::is_scalar_v<Right>;

		// Returns true if [Left] is a scalar type and Right inherits from the [strict_cpp_base_t] type.
		template <typename Left, typename Right>
		concept is_qualified_operator_right_only = std::is_scalar_v<Left> && is_qualified_operator<Right>;

		// Returns true if all [Types] inherits from the [strict_cpp_integral_base_t] type.
		template <typename... Types>
		concept is_qualified_integral_operator = (std::is_base_of_v<STRICT_CPP_NAMESPACE::detail::strict_cpp_integral_base_t, Types> && ...);

		// Returns true if [Left] inherits from the [strict_cpp_integral_base_t] type and [Right] is a scalar type.
		template <typename Left, typename Right>
		concept is_qualified_integral_operator_left_only = is_qualified_integral_operator<Left> && std::is_scalar_v<Right>;

		// Returns true if [Left] is a scalar type and [Right] inherits from the [strict_cpp_integral_base_t] type.
		template <typename Left, typename Right>
		concept is_qualified_integral_operator_right_only = std::is_scalar_v<Left> && is_qualified_integral_operator<Right>;

		// Returns true if any of [Types] inherits from the [strict_cpp_float_base_t] type.
		template <typename... Types>
		concept is_qualified_float_operator = (std::is_base_of_v<strict_cpp_float_base_t, Types> && ...);

		// Returns true if [Left] inherits from the [strict_cpp_float_base_t] type and [Right] is a arithmetic type.
		template <typename Left, typename Right>
		concept is_qualified_float_operator_left_only = is_qualified_float_operator<Left> && (std::is_arithmetic_v<Right> || std::is_base_of_v<STRICT_CPP_NAMESPACE::detail::strict_cpp_integral_base_t, Right>);

		// Returns true if [Left] is a arithmetic type and [Right] inherits from the [strict_cpp_float_base_t] type.
		template <typename Left, typename Right>
		concept is_qualified_float_operator_right_only = (std::is_arithmetic_v<Left> || std::is_base_of_v<STRICT_CPP_NAMESPACE::detail::strict_cpp_integral_base_t, Left>) && is_qualified_float_operator<Right>;

		// Returns true if [Other] is integral or integral strict type.
		template <typename Other>
		concept is_qualified_integral_assignment_operator = std::is_integral_v<Other> || std::is_base_of_v<Other, STRICT_CPP_NAMESPACE::detail::strict_cpp_integral_base_t>;

		// Returns true if [Other] is a float or float strict type.
		template <typename Other>
		concept is_qualified_float_assignment_operator = std::is_floating_point_v<Other> || std::is_base_of_v<Other, STRICT_CPP_NAMESPACE::detail::strict_cpp_float_base_t>;

		// Returns true if [Type] has a subscript operator ([]).
		template <typename Type, typename IndexType>
		concept has_subscript_operator
			= requires (Type t, IndexType&& i) { t.operator[](i); }
		  || requires (Type t, const IndexType& i) { t.operator[](i); };

		// Returns true if [Other] inherits from the [strict_cpp_base_t], or is convertible to [Type].
		template <typename Type, typename Other>
		concept is_qualified_conversion_fuction = std::is_base_of_v<STRICT_CPP_NAMESPACE::detail::strict_cpp_base_t, Other> || std::is_convertible_v<Type, Other>;

		// Returns true if [Type] can be converted to a std::string.
		template <typename Type>
		concept can_convert_to_string = std::is_convertible_v<Type, std::string>;

		// Returns true if [Type] can be converted to a std::string using std::to_string().
		template <typename Type>
		concept can_convert_to_string_function = requires (Type t) { std::to_string(t); };

		// Returns true if [Type] can be converted to a std::wstring.
		template <typename Type>
		concept can_convert_to_wstring = std::is_convertible_v<Type, std::wstring>;

		// Returns true if [Type] can be converted to a std::wstring using std::to_wstring().
		template <typename Type>
		concept can_convert_to_wstring_function = requires (Type t) { std::to_wstring(t); };

		// Returns true if [Type] can be converted to a std::string.
		template <typename Type>
		concept can_stringify = can_convert_to_string<Type> || can_convert_to_string_function<Type>;

		// Returns true if [Type] can be converted to a std::wstring.
		template <typename Type>
		concept can_wstringify = can_convert_to_wstring<Type> || can_convert_to_wstring_function<Type>;

		// Returns true if [Type] has a .data() function.
		template <typename Type>
		concept has_data_function = requires (Type t) { t.data(); };

		// Returns true if [Type] has a .begin() function.
		template <typename Type>
		concept has_begin_function = requires (Type t) { t.begin(); };

		// Returns true if [Type] has a .end() function.
		template <typename Type>
		concept has_end_function = requires (Type t) { t.end(); };

		// Returns true if [Type] has a .rbegin() function.
		template <typename Type>
		concept has_rbegin_function = requires (Type t) { t.rbegin(); };

		// Returns true if [Type] has a .rend() function.
		template <typename Type>
		concept has_rend_function = requires (Type t) { t.rend(); };

		// Returns true if [Type] has a .cbegin() function.
		template <typename Type>
		concept has_cbegin_function = requires (Type t) { t.cbegin(); };

		// Returns true if [Type] has a .cend() function.
		template <typename Type>
		concept has_cend_function = requires (Type t) { t.cend(); };

		// Returns true if [Type] has a .crbegin() function.
		template <typename Type>
		concept has_crbegin_function = requires (Type t) { t.crbegin(); };

		// Returns true if [Type] has a .crend() function.
		template <typename Type>
		concept has_crend_function = requires (Type t) { t.crend(); };

		// Returns true if [Type] has a ._Unchecked_begin() function.
		template <typename Type>
		concept has_Unchecked_begin_function = requires (Type t) { t._Unchecked_begin(); };

		// Returns true if [Type] has a ._Unchecked_end() function.
		template <typename Type>
		concept has_Unchecked_end_function = requires (Type t) { t._Unchecked_end(); };

		// Returns true if [Type] has a .empty() function.
		template <typename Type>
		concept has_empty_function = requires (Type t) { t.empty(); };

		// Returns true if [Type] has a .size() function.
		template <typename Type>
		concept has_size_function = requires (Type t) { t.size(); };

		// Returns true if [Type] has a .max_size() function.
		template <typename Type>
		concept has_max_size_function = requires (Type t) { t.max_size(); };

		// Returns true if [Type] has a .capacity() function.
		template <typename Type>
		concept has_capacity_function = requires (Type t) { t.capacity(); };

		// Returns true if [Type] has a .front() function.
		template <typename Type>
		concept has_front_function = requires (Type t) { t.front(); };

		// Returns true if [Type] has a .back() function.
		template <typename Type>
		concept has_back_function = requires (Type t) { t.back(); };

		// Returns true if [Type] has a .get_allocator() function.
		template <typename Type>
		concept has_get_allocator_function = requires (Type t) { t.get_allocator(); };
	}

	// =============================================================================
	// Operators for both integral and floating-point types
	// =============================================================================

	template <typename Type>
		requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator<Type>
	inline constexpr Type operator++(Type& value) noexcept {
		return static_cast<Type>(++value.value);
	}

	template <typename Type>
		requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator<Type>
	inline constexpr Type operator++(Type& value, auto) noexcept {
		return static_cast<Type>(value.value++);
	}

	template <typename Type>
		requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator<Type>
	inline constexpr Type operator--(Type& value) noexcept {
		return static_cast<Type>(--value.value);
	}

	template <typename Type>
		requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator<Type>
	inline constexpr Type operator--(Type& value, auto) noexcept {
		return static_cast<Type>(value.value--);
	}

	template <typename Type>
		requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator<Type>
	inline constexpr Type operator+(const Type value) noexcept {
		return static_cast<Type>(+value.value);
	}

	template <typename Type>
		requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator<Type>
	inline constexpr Type operator-(const Type value) noexcept {
		return static_cast<Type>(-value.value);
	}

	STRICT_CPP_DEFINE_COMPARISON_OPERATOR(==)
	STRICT_CPP_DEFINE_COMPARISON_OPERATOR(!=)
	STRICT_CPP_DEFINE_COMPARISON_OPERATOR(>)
	STRICT_CPP_DEFINE_COMPARISON_OPERATOR(<)
	STRICT_CPP_DEFINE_COMPARISON_OPERATOR(>=)
	STRICT_CPP_DEFINE_COMPARISON_OPERATOR(<=)
	STRICT_CPP_DEFINE_COMPARISON_OPERATOR(<=>)

	// ==========================================================================
	// Integral-only operators
	// ==========================================================================

	template <typename Type>
		requires STRICT_CPP_NAMESPACE::detail::is_qualified_integral_operator<Type>
	inline constexpr Type operator~(const Type value) noexcept {
		return static_cast<Type>(~value.value);
	}

	STRICT_CPP_DEFINE_INTEGRAL_OPERATOR(+)
	STRICT_CPP_DEFINE_INTEGRAL_OPERATOR(-)
	STRICT_CPP_DEFINE_INTEGRAL_OPERATOR(*)
	STRICT_CPP_DEFINE_INTEGRAL_OPERATOR(/)
	STRICT_CPP_DEFINE_INTEGRAL_OPERATOR(&)
	STRICT_CPP_DEFINE_INTEGRAL_OPERATOR(%)
	STRICT_CPP_DEFINE_INTEGRAL_OPERATOR(|)
	STRICT_CPP_DEFINE_INTEGRAL_OPERATOR(^)
	STRICT_CPP_DEFINE_INTEGRAL_OPERATOR(>>)
	STRICT_CPP_DEFINE_INTEGRAL_OPERATOR(<<)

	STRICT_CPP_DEFINE_COMPOUND_INTEGRAL_OPERATOR(+=)
	STRICT_CPP_DEFINE_COMPOUND_INTEGRAL_OPERATOR(-=)
	STRICT_CPP_DEFINE_COMPOUND_INTEGRAL_OPERATOR(*=)
	STRICT_CPP_DEFINE_COMPOUND_INTEGRAL_OPERATOR(/=)
	STRICT_CPP_DEFINE_COMPOUND_INTEGRAL_OPERATOR(&=)
	STRICT_CPP_DEFINE_COMPOUND_INTEGRAL_OPERATOR(%=)
	STRICT_CPP_DEFINE_COMPOUND_INTEGRAL_OPERATOR(|=)
	STRICT_CPP_DEFINE_COMPOUND_INTEGRAL_OPERATOR(^=)
	STRICT_CPP_DEFINE_COMPOUND_INTEGRAL_OPERATOR(>>=)
	STRICT_CPP_DEFINE_COMPOUND_INTEGRAL_OPERATOR(<<=)

	// ==========================================================================
	// Float-only operators
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

	STRICT_CPP_DEFINE_FLOAT_OPERATOR(+)
	STRICT_CPP_DEFINE_FLOAT_OPERATOR(-)
	STRICT_CPP_DEFINE_FLOAT_OPERATOR(*)
	STRICT_CPP_DEFINE_FLOAT_OPERATOR(/)

	STRICT_CPP_DEFINE_COMPOUND_FLOAT_OPERATOR(+=)
	STRICT_CPP_DEFINE_COMPOUND_FLOAT_OPERATOR(-=)
	STRICT_CPP_DEFINE_COMPOUND_FLOAT_OPERATOR(*=)
	STRICT_CPP_DEFINE_COMPOUND_FLOAT_OPERATOR(/=)

	template <typename Type, typename... QualifiedTypes>
		requires std::is_integral_v<Type> && std::is_trivial_v<Type>
	struct strict_integral_type : STRICT_CPP_NAMESPACE::detail::strict_integral_value_t<Type> {
			/// @brief Default constructor.
			inline constexpr strict_integral_type() noexcept = default;

			/// @brief Implicit copy constructor.
			/// @tparam Other The implicitly-convertible type.
			/// @param other The implicitly-convertible value.
			template <typename Other>
				requires STRICT_CPP_NAMESPACE::detail::is_qualified_implicit_constructor<Other, Type, QualifiedTypes...>
			inline constexpr strict_integral_type(const Other other) noexcept :
				STRICT_CPP_NAMESPACE::detail::strict_integral_value_t<Type>(static_cast<Type>(other)) { }

			/// @brief Explicit copy constructor.
			/// @tparam Other The explicitly-convertible type.
			/// @param other The explicitly-convertible value.
			template <typename Other>
				requires STRICT_CPP_NAMESPACE::detail::is_qualified_explicit_constructor<Other, Type>
			inline constexpr explicit strict_integral_type(const Other other) noexcept :
				STRICT_CPP_NAMESPACE::detail::strict_integral_value_t<Type>(static_cast<Type>(other)) { }

			/// @brief Copy assignment operator.
			/// @tparam Other The assignment type.
			/// @returns strict_integral_type&
			template <typename Other>
				requires STRICT_CPP_NAMESPACE::detail::is_qualified_integral_assignment_operator<Other>
			inline constexpr strict_integral_type& operator=(const Other other) noexcept {
				if constexpr (std::is_integral_v<Other>) this->value = other;
				else this->value = other.value;
				return *this;
			}

			/// @brief Conversion function.
			/// @tparam Other The type to convert to.
			/// @returns Other
			template <typename Other>
				requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator<Other> || std::is_convertible_v<Type, Other>
			inline constexpr Other as() const noexcept {
				return static_cast<Other>(this->value);
			}
	};

	template <typename Type, typename... QualifiedTypes>
		requires std::is_floating_point_v<Type> && std::is_trivial_v<Type>
	struct strict_float_type : STRICT_CPP_NAMESPACE::detail::strict_float_value_t<Type> {
			/// @brief Default constructor.
			inline constexpr strict_float_type() noexcept = default;

			/// @brief Implicit copy constructor.
			/// @tparam Other The implicitly-convertible type.
			/// @param other The implicitly-convertible value.
			template <typename Other>
				requires STRICT_CPP_NAMESPACE::detail::is_qualified_implicit_constructor<Other, Type, QualifiedTypes...>
			inline constexpr strict_float_type(const Other other) noexcept :
				STRICT_CPP_NAMESPACE::detail::strict_float_value_t<Type>(static_cast<Type>(other)) { }

			/// @brief Explicit copy constructor.
			/// @tparam Other The explicitly-convertible type.
			/// @param other The explicitly-convertible value.
			template <typename Other>
				requires STRICT_CPP_NAMESPACE::detail::is_qualified_explicit_constructor<Other, Type>
			inline constexpr explicit strict_float_type(const Other other) noexcept :
				STRICT_CPP_NAMESPACE::detail::strict_float_value_t<Type>(static_cast<Type>(other)) { }

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

			/// @brief Conversion function.
			/// @tparam Other The type to convert to.
			/// @returns Other
			template <typename Other>
				requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator<Other> || std::is_convertible_v<Type, Other>
			inline constexpr Other as() const noexcept {
				return static_cast<Other>(this->value);
			}
	};

	template <typename Type, typename... QualifiedTypes>
		requires (sizeof(Type) != 0) && (!std::is_reference_v<Type>)
	struct strict_alias_type : STRICT_CPP_NAMESPACE::detail::strict_cpp_alias_base_t {
			using type = Type;
			Type value = {};

			/// @brief Default constructor.
			inline constexpr strict_alias_type() noexcept = default;

			/// @brief Move construct-in-place constructor.
			/// @tparam Args Constructor arguments.
			template <typename... Args>
				requires std::is_constructible_v<Type, Args...>
			inline constexpr strict_alias_type(Args&&... args) noexcept :
				value(std::move(args)...) { }

			/// @brief Copy construct-in-place constructor.
			/// @tparam Args Constructor arguments to copy.
			template <typename... Args>
				requires std::is_constructible_v<Type, Args...>
			inline constexpr strict_alias_type(const Args&... args) noexcept :
				value(std::forward<const Args>(args)...) { }

			/// @brief Move-assignment operator.
			/// @tparam Other The assignment type.
			/// @param other The move-assignment value.
			/// @returns auto&
			template <typename Other>
				requires std::is_base_of_v<Other, STRICT_CPP_NAMESPACE::detail::strict_cpp_alias_base_t> && std::is_convertible_v<Other, Type>
			inline constexpr auto& operator=(Other&& other) noexcept {
				this->value = std::move(other.value);
				return *this;
			}

			/// @brief Move-assignment operator.
			/// @tparam Other The assignment type.
			/// @param other The move-assignment value.
			/// @returns auto&
			template <typename Other>
				requires std::is_convertible_v<Other, Type> && (!std::is_base_of_v<Other, STRICT_CPP_NAMESPACE::detail::strict_cpp_alias_base_t>)
			inline constexpr auto& operator=(Other&& other) noexcept {
				this->value = std::move(other);
				return *this;
			}

			/// @brief Copy-assignment operator.
			/// @tparam Other The assignment type.
			/// @param other The copy-assignment value.
			/// @returns auto&
			template <typename Other>
				requires std::is_base_of_v<Other, STRICT_CPP_NAMESPACE::detail::strict_cpp_alias_base_t> && std::is_convertible_v<Other, Type>
			inline constexpr auto& operator=(const Other& other) noexcept {
				this->value = std::forward<Other>(other.value);
				return *this;
			}

			/// @brief Copy-assignment operator.
			/// @tparam Other The assignment type.
			/// @param other The copy-assignment value.
			/// @returns auto&
			template <typename Other>
				requires std::is_convertible_v<Other, Type> && (!std::is_base_of_v<Other, STRICT_CPP_NAMESPACE::detail::strict_cpp_alias_base_t>)
			inline constexpr auto& operator=(const Other& other) noexcept {
				this->value = std::forward<Other>(other);
				return *this;
			}

			// @brief Implicit ref conversion operator that converts to the same encapsulated type only.
			// @returns Type&
			inline constexpr operator Type&() noexcept { return this->value; }

			// @brief Implicit const ref conversion operator that converts to the same encapsulated type only.
			// @returns const Type&
			inline constexpr operator const Type&() const noexcept { return this->value; }

			/// @brief Explicit copy conversion operator.
			/// @tparam Other The type to convert to.
			/// @returns Other
			template <typename Other>
				requires std::is_convertible_v<Type, Other>
			inline constexpr explicit operator Other() noexcept {
				return static_cast<Other>(this->value);
			}

			/// @brief Explicit const copy conversion operator.
			/// @tparam Other The type to convert to.
			/// @returns Other
			template <typename Other>
				requires std::is_convertible_v<Type, Other>
			inline constexpr explicit operator Other() const noexcept {
				return static_cast<Other>(this->value);
			}

			/// @brief Indirect member access operator.
			/// @returns Type
			template <typename _ = void>
				requires std::is_pointer_v<Type>
			inline constexpr Type operator->() noexcept {
				return this->value;
			}

			/// @brief Indirect const member access operator.
			/// @returns const Type
			template <typename _ = void>
				requires std::is_pointer_v<Type>
			inline constexpr Type operator->() const noexcept {
				return this->value;
			}

			/// @brief Indirect member access operator.
			/// @returns Type*
			template <typename _ = void>
				requires std::is_class_v<Type>
			inline constexpr Type* operator->() noexcept {
				return &this->value;
			}

			/// @brief Indirect const member access operator.
			/// @returns Type*
			template <typename _ = void>
				requires std::is_class_v<Type>
			inline constexpr Type* operator->() const noexcept {
				return &this->value;
			}

			/// @brief Subscript operator.
			/// @returns auto
			template <typename Indexer>
				requires STRICT_CPP_NAMESPACE::detail::has_subscript_operator<Type, Indexer> && (!std::is_reference_v<decltype(std::declval<Type>().operator[](Indexer{}))>)
			inline constexpr auto operator[](Indexer && index) const noexcept {
				return this->value[std::move(index)];
			}

			/// @brief Subscript operator.
			/// @returns auto
			template <typename Indexer>
				requires STRICT_CPP_NAMESPACE::detail::has_subscript_operator<Type, Indexer> && (!std::is_reference_v<decltype(std::declval<Type>().operator[](Indexer{}))>)
			inline constexpr auto operator[](const Indexer & index) const noexcept {
				return this->value[std::forward<const Indexer>(index)];
			}

			/// @brief Subscript operator.
			/// @returns auto&
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
			/// @returns Other
			template <typename Other>
				requires STRICT_CPP_NAMESPACE::detail::is_qualified_conversion_fuction<Type, Other>
			inline constexpr Other as() const noexcept {
				return static_cast<const Other>(this->value);
			}

			// Note: some of the below functions may be unecessary, but for now they will stay.

			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::has_data_function<Type>
			inline constexpr auto data() noexcept { return this->value.data(); }

			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::has_data_function<Type>
			inline constexpr auto data() const noexcept { return this->value.data(); }

			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::has_begin_function<Type>
			inline constexpr auto begin() noexcept { return this->value.begin(); }

			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::has_begin_function<Type>
			inline constexpr auto begin() const noexcept { return this->value.begin(); }

			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::has_end_function<Type>
			inline constexpr auto end() noexcept { return this->value.end(); }

			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::has_end_function<Type>
			inline constexpr auto end() const noexcept { return this->value.end(); }

			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::has_rbegin_function<Type>
			inline constexpr auto rbegin() noexcept { return this->value.rbegin(); }

			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::has_rbegin_function<Type>
			inline constexpr auto rbegin() const noexcept { return this->value.rbegin(); }

			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::has_rend_function<Type>
			inline constexpr auto rend() noexcept { return this->value.rend(); }

			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::has_rend_function<Type>
			inline constexpr auto rend() const noexcept { return this->value.rend(); }

			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::has_cbegin_function<Type>
			inline constexpr auto cbegin() const noexcept { return this->value.cbegin(); }

			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::has_cend_function<Type>
			inline constexpr auto cend() const noexcept { return this->value.cend(); }

			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::has_crbegin_function<Type>
			inline constexpr auto crbegin() const noexcept { return this->value.crbegin(); }

			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::has_crend_function<Type>
			inline constexpr auto crend() const noexcept { return this->value.crend(); }

			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::has_Unchecked_begin_function<Type>
			inline constexpr auto _Unchecked_begin() noexcept { return this->value._Unchecked_begin(); }

			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::has_Unchecked_begin_function<Type>
			inline constexpr auto _Unchecked_begin() const noexcept { return this->value._Unchecked_begin(); }

			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::has_Unchecked_end_function<Type>
			inline constexpr auto _Unchecked_end() noexcept { return this->value._Unchecked_end(); }

			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::has_Unchecked_end_function<Type>
			inline constexpr auto _Unchecked_end() const noexcept { return this->value._Unchecked_end(); }

			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::has_empty_function<Type>
			inline constexpr auto empty() const noexcept { return this->value.empty(); }

			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::has_size_function<Type>
			inline constexpr auto size() const noexcept { return this->value.size(); }

			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::has_max_size_function<Type>
			inline constexpr auto max_size() const noexcept { return this->value.max_size(); }

			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::has_capacity_function<Type>
			inline constexpr auto capacity() const noexcept { return this->value.capacity(); }

			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::has_front_function<Type>
			inline constexpr auto front() noexcept { return this->value.front(); }

			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::has_front_function<Type>
			inline constexpr auto front() const noexcept { return this->value.front(); }

			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::has_back_function<Type>
			inline constexpr auto back() noexcept { return this->value.back(); }

			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::has_back_function<Type>
			inline constexpr auto back() const noexcept { return this->value.back(); }

			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::has_get_allocator_function<Type>
			inline constexpr auto get_allocator() const noexcept { return this->value.get_allocator(); }

		protected:
			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::can_stringify<Type>
			inline std::string m_get_string_internal() const noexcept {
				if constexpr (STRICT_CPP_NAMESPACE::detail::can_convert_to_string<Type>) return this->value;
				else return std::to_string(this->value);
			}

			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::can_wstringify<Type>
			inline std::wstring m_get_wstring_internal() const noexcept {
				if constexpr (STRICT_CPP_NAMESPACE::detail::can_convert_to_wstring<Type>) return this->value;
				else return std::to_wstring(this->value);
			}

			template <typename _ = void>
				requires (!STRICT_CPP_NAMESPACE::detail::can_stringify<Type>)
			inline std::string m_get_string_internal() const noexcept {
				return "";
			}

			template <typename _ = void>
				requires (!STRICT_CPP_NAMESPACE::detail::can_wstringify<Type>)
			inline std::wstring m_get_wstring_internal() const noexcept {
				return L"";
			}
	};
}

#define STRICT_CPP_DEFINE_FORMATTER(TYPE)                                                                                                                                        \
	template <>                                                                                                                                                                   \
	struct _STD formatter<STRICT_CPP_NAMESPACE::TYPE> {                                                                                                                           \
			inline constexpr auto parse(const _STD format_parse_context& context) const noexcept { return context.begin(); }                                                        \
			inline constexpr auto parse(const _STD wformat_parse_context& context) const noexcept { return context.begin(); }                                                       \
			inline auto				 format(const STRICT_CPP_NAMESPACE::TYPE value, _STD format_context& context) const { return _STD format_to(context.out(), "{}", value.value); }   \
			inline auto				 format(const STRICT_CPP_NAMESPACE::TYPE value, _STD wformat_context& context) const { return _STD format_to(context.out(), L"{}", value.value); } \
	};

#define STRICT_CPP_DEFINE_INTEGRAL_TYPE(NAME, TYPE, QUALIFIED_TYPES...)                                    \
	namespace STRICT_CPP_NAMESPACE {                                                                        \
		struct NAME : STRICT_CPP_NAMESPACE::strict_integral_type<TYPE, QUALIFIED_TYPES> {                    \
				using STRICT_CPP_NAMESPACE::strict_integral_type<TYPE, QUALIFIED_TYPES>::strict_integral_type; \
				using STRICT_CPP_NAMESPACE::strict_integral_type<TYPE, QUALIFIED_TYPES>::operator=;            \
		};                                                                                                   \
	}                                                                                                       \
	STRICT_CPP_DEFINE_FORMATTER(NAME)

#define STRICT_CPP_DEFINE_FLOAT_TYPE(NAME, TYPE, QUALIFIED_TYPES...)                                 \
	namespace STRICT_CPP_NAMESPACE {                                                                  \
		struct NAME : STRICT_CPP_NAMESPACE::strict_float_type<TYPE, QUALIFIED_TYPES> {                 \
				using STRICT_CPP_NAMESPACE::strict_float_type<TYPE, QUALIFIED_TYPES>::strict_float_type; \
				using STRICT_CPP_NAMESPACE::strict_float_type<TYPE, QUALIFIED_TYPES>::operator=;         \
		};                                                                                             \
	}                                                                                                 \
	STRICT_CPP_DEFINE_FORMATTER(NAME)

#define STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(NAME, QUALIFIED_TYPES...)                               \
	namespace STRICT_CPP_NAMESPACE {                                                                     \
		template <typename T>                                                                             \
			requires std::is_integral_v<T>                                                                 \
		struct NAME : STRICT_CPP_NAMESPACE::strict_integral_type<T, QUALIFIED_TYPES> {                    \
				using STRICT_CPP_NAMESPACE::strict_integral_type<T, QUALIFIED_TYPES>::strict_integral_type; \
				using STRICT_CPP_NAMESPACE::strict_integral_type<T, QUALIFIED_TYPES>::operator=;            \
		};                                                                                                \
	}                                                                                                    \
	STRICT_CPP_DEFINE_FORMATTER(NAME<char>)                                                              \
	STRICT_CPP_DEFINE_FORMATTER(NAME<unsigned char>)                                                     \
	STRICT_CPP_DEFINE_FORMATTER(NAME<unsigned short>)                                                    \
	STRICT_CPP_DEFINE_FORMATTER(NAME<unsigned int>)                                                      \
	STRICT_CPP_DEFINE_FORMATTER(NAME<unsigned long>)                                                     \
	STRICT_CPP_DEFINE_FORMATTER(NAME<unsigned long long>)                                                \
	STRICT_CPP_DEFINE_FORMATTER(NAME<signed char>)                                                       \
	STRICT_CPP_DEFINE_FORMATTER(NAME<signed short>)                                                      \
	STRICT_CPP_DEFINE_FORMATTER(NAME<signed int>)                                                        \
	STRICT_CPP_DEFINE_FORMATTER(NAME<signed long>)                                                       \
	STRICT_CPP_DEFINE_FORMATTER(NAME<signed long long>)

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

#define STRICT_CPP_DEFINE_ALIAS_TYPE(NAME, TYPE)                                                                                                                                         \
	namespace STRICT_CPP_NAMESPACE {                                                                                                                                                      \
		struct NAME : STRICT_CPP_NAMESPACE::strict_alias_type<TYPE> {                                                                                                                      \
				using Type = TYPE;                                                                                                                                                           \
				using STRICT_CPP_NAMESPACE::strict_alias_type<TYPE>::strict_alias_type;                                                                                                      \
				using STRICT_CPP_NAMESPACE::strict_alias_type<TYPE>::operator=;                                                                                                              \
				using STRICT_CPP_NAMESPACE::strict_alias_type<TYPE>::operator->;                                                                                                             \
				using STRICT_CPP_NAMESPACE::strict_alias_type<TYPE>::operator[];                                                                                                             \
                                                                                                                                                                                         \
				template <typename _ = void>                                                                                                                                                 \
					requires STRICT_CPP_NAMESPACE::detail::can_stringify<TYPE>                                                                                                                \
				inline std::string to_string() const noexcept {                                                                                                                              \
					return this->m_get_string_internal();                                                                                                                                     \
				}                                                                                                                                                                            \
                                                                                                                                                                                         \
				template <typename _ = void>                                                                                                                                                 \
					requires STRICT_CPP_NAMESPACE::detail::can_stringify_wide<TYPE>                                                                                                           \
				inline std::wstring to_wstring() const noexcept {                                                                                                                            \
					return this->m_get_wstring_internal();                                                                                                                                    \
				}                                                                                                                                                                            \
                                                                                                                                                                                         \
				template <typename _ = void>                                                                                                                                                 \
				inline std::string to_string() const noexcept {                                                                                                                              \
					return "::" #NAME "::strict_alias_type<" #TYPE ">";                                                                                                                       \
				}                                                                                                                                                                            \
                                                                                                                                                                                         \
				template <typename _ = void>                                                                                                                                                 \
				inline std::wstring to_wstring() const noexcept {                                                                                                                            \
					return L"::" L#NAME L"::strict_alias_type<" L#TYPE L">";                                                                                                                  \
				}                                                                                                                                                                            \
		};                                                                                                                                                                                 \
	}                                                                                                                                                                                     \
	template <>                                                                                                                                                                           \
	struct _STD formatter<STRICT_CPP_NAMESPACE::NAME> {                                                                                                                                   \
			inline constexpr auto parse(const _STD format_parse_context& context) const noexcept { return context.begin(); }                                                                \
			inline constexpr auto parse(const _STD wformat_parse_context& context) const noexcept { return context.begin(); }                                                               \
			inline auto				 format(const STRICT_CPP_NAMESPACE::NAME& value, _STD format_context& context) const { return _STD format_to(context.out(), "{}", value.to_string()); }    \
			inline auto				 format(const STRICT_CPP_NAMESPACE::NAME& value, _STD wformat_context& context) const { return _STD format_to(context.out(), L"{}", value.to_wstring()); } \
	};

// =============================================================================
// Pre-defined types
// =============================================================================

// Common integral types:
STRICT_CPP_DEFINE_INTEGRAL_TYPE(char_t, char);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(schar_t, signed char);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uchar_t, unsigned char);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(short_t, short);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(ushort_t, unsigned short);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int_t, int);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint_t, unsigned int);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(long_t, long);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(ulong_t, unsigned long);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(long_long_t, long long);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(ulong_long_t, unsigned long long);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(size_t, std::size_t);

STRICT_CPP_DEFINE_INTEGRAL_TYPE(_wchar_t, wchar_t);
#if defined(__cpp_char8_t)
STRICT_CPP_DEFINE_INTEGRAL_TYPE(_char8_t, char8_t);
#endif
STRICT_CPP_DEFINE_INTEGRAL_TYPE(_char16_t, char16_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(_char32_t, char32_t);

STRICT_CPP_DEFINE_INTEGRAL_TYPE(size8_t, std::uint8_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(size16_t, std::uint16_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(size32_t, std::uint32_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(size64_t, std::uint64_t);

STRICT_CPP_DEFINE_INTEGRAL_TYPE(int8_t, std::int8_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int16_t, std::int16_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int32_t, std::int32_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int64_t, std::int64_t);

STRICT_CPP_DEFINE_INTEGRAL_TYPE(int_least8_t, std::int_least8_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int_least16_t, std::int_least16_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int_least32_t, std::int_least32_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int_least64_t, std::int_least64_t);

STRICT_CPP_DEFINE_INTEGRAL_TYPE(int_fast8_t, std::int_fast8_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int_fast16_t, std::int_fast16_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int_fast32_t, std::int_fast32_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int_fast64_t, std::int_fast64_t);

STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint8_t, std::uint8_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint16_t, std::uint16_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint32_t, std::uint32_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint64_t, std::uint64_t);

STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint_least8_t, std::uint_least8_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint_least16_t, std::uint_least16_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint_least32_t, std::uint_least32_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint_least64_t, std::uint_least64_t);

STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint_fast8_t, std::uint_fast8_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint_fast16_t, std::uint_fast16_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint_fast32_t, std::uint_fast32_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint_fast64_t, std::uint_fast64_t);

STRICT_CPP_DEFINE_INTEGRAL_TYPE(intmax_t, std::intmax_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uintmax_t, std::uintmax_t);

STRICT_CPP_DEFINE_INTEGRAL_TYPE(intptr_t, std::intptr_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uintptr_t, std::uintptr_t);

STRICT_CPP_DEFINE_INTEGRAL_TYPE(byte_t, std::uint8_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(sbyte_t, std::int8_t);

// =============================================================================
// Dynamic "any" integral types
// =============================================================================

// int

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_int_t,
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
	any_signed_int_t,
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
	any_unsigned_int_t,
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
	std::uint8_t,
	STRICT_CPP_NAMESPACE::schar_t,
	STRICT_CPP_NAMESPACE::uchar_t,
	STRICT_CPP_NAMESPACE::int8_t,
	STRICT_CPP_NAMESPACE::uint8_t
);

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_signed_int8_t,
	std::int8_t,
	STRICT_CPP_NAMESPACE::schar_t,
	STRICT_CPP_NAMESPACE::int8_t
);

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_unsigned_int8_t,
	std::uint8_t,
	STRICT_CPP_NAMESPACE::uchar_t,
	STRICT_CPP_NAMESPACE::uint8_t
);

// int16

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_int16_t,
	std::int16_t,
	std::uint16_t,
	STRICT_CPP_NAMESPACE::short_t,
	STRICT_CPP_NAMESPACE::ushort_t,
	STRICT_CPP_NAMESPACE::int16_t,
	STRICT_CPP_NAMESPACE::uint16_t
);

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_signed_int16_t,
	std::int16_t,
	STRICT_CPP_NAMESPACE::short_t,
	STRICT_CPP_NAMESPACE::int16_t
);

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_unsigned_int16_t,
	std::uint16_t,
	STRICT_CPP_NAMESPACE::ushort_t,
	STRICT_CPP_NAMESPACE::uint16_t
);

// int32

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_int32_t,
	std::int32_t,
	std::uint32_t,
	STRICT_CPP_NAMESPACE::int_t,
	STRICT_CPP_NAMESPACE::uint_t,
	STRICT_CPP_NAMESPACE::int32_t,
	STRICT_CPP_NAMESPACE::uint32_t
);

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_signed_int32_t,
	std::int32_t,
	STRICT_CPP_NAMESPACE::int_t,
	STRICT_CPP_NAMESPACE::int32_t
);

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_unsigned_int32_t,
	std::uint32_t,
	STRICT_CPP_NAMESPACE::uint_t,
	STRICT_CPP_NAMESPACE::uint32_t
);

// int64

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_int64_t,
	std::int64_t,
	std::uint64_t,
	STRICT_CPP_NAMESPACE::long_t,
	STRICT_CPP_NAMESPACE::ulong_t,
	STRICT_CPP_NAMESPACE::int64_t,
	STRICT_CPP_NAMESPACE::uint64_t
);

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_signed_int64_t,
	std::int64_t,
	STRICT_CPP_NAMESPACE::long_t,
	STRICT_CPP_NAMESPACE::int64_t
);

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
	any_unsigned_int64_t,
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
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(begin_size);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(end_size);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(src_size);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(dst_size);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(lhs_size);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(rhs_size);

// offset_size_t
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(offset_size);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(begin_offset_size);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(end_offset_size);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(src_offset_size);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(dst_offset_size);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(lhs_offset_size);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(rhs_offset_size);

// count_t
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(count);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(begin_count);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(end_count);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(src_count);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(dst_count);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(lhs_count);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(rhs_count);

// offset_count_t
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(offset_count);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(begin_offset_count);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(end_offset_count);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(src_offset_count);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(dst_offset_count);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(lhs_offset_count);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(rhs_offset_count);

// offset_t
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(offset);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(begin_offset);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(end_offset);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(src_offset);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(dst_offset);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(lhs_offset);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(rhs_offset);

// index_t
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(index);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(begin_index);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(end_index);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(src_index);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(dst_index);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(lhs_index);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(rhs_index);

// index_offset_t
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(index_offset);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(begin_index_offset);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(end_index_offset);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(src_index_offset);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(dst_index_offset);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(lhs_index_offset);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(rhs_index_offset);

// others
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(capacity);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(length);
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-pragmas"

#pragma pop_macro("min")
#pragma pop_macro("max")

#pragma GCC diagnostic pop
