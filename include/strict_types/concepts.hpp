#pragma once

#include "strict_types/common.hpp"

#include <string>
#include <type_traits>

/*
	concepts.hpp description:
		This header file contains a wide range of c++20 concepts,
		hidden inside the "detail" namespace. These concepts are to be used
		for all types of strict_types types.
*/

namespace STRICT_CPP_NAMESPACE {
	namespace detail {
		// Returns true if [Type] inherits from the base class strict_cpp_base_t.
		template <typename Type>
		concept is_strict_type = std::is_base_of_v<STRICT_CPP_NAMESPACE::detail::strict_cpp_base_t, Type>;

		// Returns true if [Type] inherits from the base class strict_cpp_integral_base_t.
		template <typename Type>
		concept is_strict_integral_type = std::is_base_of_v<STRICT_CPP_NAMESPACE::detail::strict_cpp_integral_base_t, Type>;

		// Returns true if [Type] inherits from the base class strict_cpp_float_base_t.
		template <typename Type>
		concept is_strict_float_type = std::is_base_of_v<STRICT_CPP_NAMESPACE::detail::strict_cpp_float_base_t, Type>;

		// Returns true if [Type] inherits from the base class strict_cpp_alias_base_t.
		template <typename Type>
		concept is_strict_alias_type = std::is_base_of_v<STRICT_CPP_NAMESPACE::detail::strict_cpp_alias_base_t, Type>;

		// Returns true if [Other] is the same as one of the [QualifiedTypes], and if [QualifiedTypes] is a non-zero length.
		template <typename Other, typename... QualifiedTypes>
		concept is_qualified_type = (sizeof...(QualifiedTypes) > 0) && (std::is_same_v<Other, QualifiedTypes> || ...);

		// Returns true if any of [Types] are trivial integral types.
		template <typename... Types>
		concept is_qualified_integral_type = (((std::is_integral_v<Types> || is_strict_integral_type<Types>) && std::is_trivial_v<Types>) || ...);

		// Returns true if any of [Types] are trivial floating-point types.
		template <typename... Types>
		concept is_qualified_float_type = (((std::is_floating_point_v<Types> || is_strict_float_type<Types>) && std::is_trivial_v<Types>) || ...);

		// Returns true if [Other] is the same as one of the [QualifiedTypes], and if [QualifiedTypes] is a non-zero length.
		template <typename Other, typename... QualifiedTypes>
		concept is_qualified_implicit_constructor = is_qualified_type<Other, QualifiedTypes...>;

		// Returns true if [Other] is convertible to [Type].
		template <typename Other, typename Type>
		concept is_qualified_explicit_constructor = (std::is_convertible_v<Other, Type> || is_strict_type<Other>) && !std::is_same_v<Other, Type>;

		// Returns true if [Type] is convertible to [Other], but is not the exact same type.
		template <typename Type, typename Other>
		concept is_qualified_explicit_conversion_operator = std::is_convertible_v<Type, Other> && !std::is_same_v<Type, Other>;

		// Returns true if all [Types] inherits from the [strict_cpp_base_t] type.
		template <typename... Types>
		concept is_qualified_operator = (is_strict_type<Types> && ...);

		// Returns true if [Left] inherits from the [strict_cpp_base_t] type and Right is a scalar type.
		template <typename Left, typename Right>
		concept is_qualified_operator_left_only = is_qualified_operator<Left> && std::is_arithmetic_v<Right>;

		// Returns true if [Left] is a scalar type and Right inherits from the [strict_cpp_base_t] type.
		template <typename Left, typename Right>
		concept is_qualified_operator_right_only = std::is_scalar_v<Left> && is_qualified_operator<Right>;

		// Returns true if all [Types] inherits from the [strict_cpp_integral_base_t] type.
		template <typename... Types>
		concept is_qualified_integral_operator = (is_strict_integral_type<Types> && ...);

		// Returns true if [Left] inherits from the [strict_cpp_integral_base_t] type and [Right] is a scalar type.
		template <typename Left, typename Right>
		concept is_qualified_integral_operator_left_only = is_qualified_integral_operator<Left> && std::is_arithmetic_v<Right>;

		// Returns true if [Left] is a scalar type and [Right] inherits from the [strict_cpp_integral_base_t] type.
		template <typename Left, typename Right>
		concept is_qualified_integral_operator_right_only = std::is_scalar_v<Left> && is_qualified_integral_operator<Right>;

		// Returns true if any of [Types] inherits from the [strict_cpp_float_base_t] type.
		template <typename... Types>
		concept is_qualified_float_operator = (is_strict_float_type<Types> && ...);

		// Returns true if [Left] inherits from the [strict_cpp_float_base_t] type and [Right] is a arithmetic type.
		template <typename Left, typename Right>
		concept is_qualified_float_operator_left_only = is_qualified_float_operator<Left> && (std::is_arithmetic_v<Right> || is_strict_integral_type<Right>);

		// Returns true if [Left] is a arithmetic type and [Right] inherits from the [strict_cpp_float_base_t] type.
		template <typename Left, typename Right>
		concept is_qualified_float_operator_right_only = (std::is_arithmetic_v<Left> || is_strict_integral_type<Left>) && is_qualified_float_operator<Right>;

		// Returns true if [Other] is integral or integral strict_types type.
		template <typename Other>
		concept is_qualified_integral_assignment_operator = std::is_integral_v<Other> || is_strict_integral_type<Other>;

		// Returns true if [Other] is a float or float strict_types type.
		template <typename Other>
		concept is_qualified_float_assignment_operator = std::is_floating_point_v<Other> || is_strict_float_type<Other>;

		// Returns true if [Type] has a subscript operator ([]).
		template <typename Type, typename IndexType>
		concept has_subscript_operator
			= requires (Type t, IndexType&& i) { t.operator[](i); }
		  || requires (Type t, const IndexType& i) { t.operator[](i); };

		// Returns true if [Other] inherits from the [strict_cpp_base_t], or is convertible to [Type].
		template <typename Type, typename Other>
		concept is_qualified_conversion_fuction = is_strict_type<Other> || std::is_convertible_v<Type, Other>;

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
}
