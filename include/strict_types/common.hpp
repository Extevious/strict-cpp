#pragma once

/*
	common.hpp description:
		This header contains the base types used for type qualifications.
		These types must be inherited in a sequential manner to conform
		to trivial-type requirements.

		Example inheritance:
			|---------[base]-----------------[type qualification]-----------------[encapsulation]---------------------[user exposed]-------|
			|                                                                                                                              |
			|   strict_cpp_base_t ------> strict_cpp_integral_base_t ---> strict_integral_type<std::size_t> -----> my_size_t               |
			|    				        |                                                                                                       |
			|   					     |----> strict_cpp_float_base_t ------> strict_float_type<double> -------------> my_double_t             |
			|   					     |                                                                                                       |
			|   					      `---> strict_cpp_alias_base_t ------> strict_alias_type<std::vector<int>> ---> my_int_vector_alias_t   |
			|                                                                                                                              |
			|------------------------------------------------------------------------------------------------------------------------------|
*/

namespace STRICT_TYPES_NAMESPACE {
	namespace detail {
		/// @brief Base type for integral or float type qualifications.
		struct strict_cpp_base_t { };

		/// @brief Base type for alias-only type qualifications.
		struct strict_cpp_alias_base_t { };

		/// @brief Base type for integral-only type qualifications.
		struct strict_cpp_integral_base_t : STRICT_TYPES_NAMESPACE::detail::strict_cpp_base_t { };

		/// @brief Base type for float-only type qualifications.
		struct strict_cpp_float_base_t : STRICT_TYPES_NAMESPACE::detail::strict_cpp_base_t { };
	}
}
