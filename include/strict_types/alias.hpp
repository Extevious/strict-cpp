#pragma once

#include "strict_types/concepts.hpp"

#include <format>
#include <initializer_list>
#include <string>
#include <type_traits>

// TODO: add iostream support
// TODO: add hash support

/*
	alias.hpp description:
		This header file contains a strict "alias-type" that is used to
		encapsulate various non-reference types, such as vector types, strings,
		pointers, etc. However, the encapsulated type cannot be a reference,
		and must be non-zero in size.

		Depending on the encapsulated type, some alias-types will dynamically
		pre-expose functions the encapsulated type has defined. For example,
		an std::vector<T> has .begin() and .end() functions. These functions
		cannot be seen by a for loop using other means aside from the .begin()
		and .end() functions dynamically defined by the strict alias-type.
		Other functions that are commonly used are also included, such as:
		.data(), .size(), .empty(), etc.
*/

namespace STRICT_CPP_NAMESPACE {
	/// @brief Strictly-typed alias encapsulation struct.
	///
	//  Requirements:
	//   - Must not encapsulate an lvalue or rvalue reference.
	//   - Encapsulated type must be non-zero in size.
	/// @tparam Type The encapsulated type.
	template <typename Type>
		requires (sizeof(Type) != 0) && (!std::is_reference_v<Type>)
	struct strict_alias_type : STRICT_CPP_NAMESPACE::detail::strict_cpp_alias_base_t {
			using type = Type;
			Type value = {};

			/// @brief Default constructor.
			inline constexpr strict_alias_type() noexcept(std::is_nothrow_default_constructible_v<Type>) = default;

			/// @brief Move constructor.
			inline constexpr strict_alias_type(strict_alias_type&& other) noexcept(std::is_nothrow_move_constructible_v<Type>) :
				value(std::move(other.value)) { }

			/// @brief Copy constructor.
			inline constexpr strict_alias_type(const strict_alias_type& other) noexcept(std::is_nothrow_copy_constructible_v<Type>) :
				value(other.value) { }

			/// @brief construct-in-place constructor.
			/// @tparam Args Constructor arguments.
			template <typename... Args>
				requires std::is_constructible_v<Type, Args...>
			inline constexpr strict_alias_type(Args&&... args) noexcept(std::is_nothrow_constructible_v<Type, Args...>) :
				value(std::forward<Args>(args)...) { }

			/// @brief Initializer list constructor.
			/// @tparam Other Constructor std::initializer_list<> arguments.
			template <typename Other>
				requires std::is_constructible_v<Type, std::initializer_list<Other>>
			inline constexpr strict_alias_type(std::initializer_list<Other> initializer_list) noexcept(std::is_nothrow_constructible_v<Type, std::initializer_list<Other>>) :
				value(initializer_list) { }

			/// @brief Move-assignment operator.
			/// @tparam Other The assignment type.
			/// @param other The move-assignment value.
			/// @returns auto&
			template <typename Other>
				requires std::is_convertible_v<Other, Type>
			inline constexpr auto& operator=(Other&& other) noexcept(std::is_nothrow_move_assignable_v<Type>) {
				if constexpr (std::is_base_of_v<Other, STRICT_CPP_NAMESPACE::detail::strict_cpp_alias_base_t>) this->value = std::move(other.value);
				else this->value = std::move(other);

				return *this;
			}

			/// @brief Copy-assignment operator.
			/// @tparam Other The assignment type.
			/// @param other The copy-assignment value.
			/// @returns auto&
			template <typename Other>
				requires std::is_convertible_v<Other, Type>
			inline constexpr auto& operator=(const Other& other) noexcept(std::is_nothrow_copy_assignable_v<Type>) {
				if constexpr (std::is_base_of_v<Other, STRICT_CPP_NAMESPACE::detail::strict_cpp_alias_base_t>) this->value = other.value;
				else this->value = other;

				return *this;
			}

			/// @brief std::initializer_list<> assignment operator.
			/// @tparam Other The assignment type.
			/// @param other The copy-assignment value.
			/// @returns auto&
			template <typename Other>
				requires std::is_convertible_v<std::initializer_list<Other>, Type>
			inline constexpr auto& operator=(std::initializer_list<Other> initializer_list) noexcept(std::is_nothrow_copy_assignable_v<Type>) {
				this->value = initializer_list;

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
				requires std::is_convertible_v<Type, Other> && (!std::is_same_v<Type, Other>)
			inline constexpr explicit operator Other() noexcept {
				return static_cast<Other>(this->value);
			}

			/// @brief Explicit const conversion operator.
			/// @tparam Other The type to convert to.
			/// @returns Other
			template <typename Other>
				requires std::is_convertible_v<Type, Other> && (!std::is_same_v<Type, Other>)
			inline constexpr explicit operator const Other() const noexcept {
				return static_cast<Other>(this->value);
			}

			/// @brief Indirect member access operator.
			/// @returns Type
			template <typename _ = void>
				requires std::is_pointer_v<Type>
			inline constexpr Type operator->() noexcept { return this->value; }

			/// @brief Const indirect member access operator.
			/// @returns const Type
			template <typename _ = void>
				requires std::is_pointer_v<Type>
			inline constexpr const Type operator->() const noexcept { return this->value; }

			/// @brief Indirect member access operator.
			/// @returns Type*
			template <typename _ = void>
				requires (!std::is_pointer_v<Type>)
			inline constexpr Type* operator->() noexcept { return std::addressof(this->value); }

			/// @brief Const indirect member access operator.
			/// @returns const Type*
			template <typename _ = void>
				requires (!std::is_pointer_v<Type>)
			inline constexpr const Type* operator->() const noexcept { return std::addressof(this->value); }

			/// @brief rvalue subscript operator.
			/// @returns auto&
			template <typename IndexType>
				requires STRICT_CPP_NAMESPACE::detail::has_subscript_operator<Type, IndexType>
			inline constexpr auto& operator[](IndexType&& index) noexcept { return this->value[std::forward<IndexType>(index)]; }

			/// @brief Const rvalue subscript operator.
			/// @returns const auto&
			template <typename IndexType>
				requires STRICT_CPP_NAMESPACE::detail::has_subscript_operator<Type, IndexType>
			inline constexpr const auto& operator[](IndexType&& index) const noexcept { return this->value[std::forward<IndexType>(index)]; }

			/// @brief lvalue subscript operator.
			/// @returns auto&
			template <typename IndexType>
				requires STRICT_CPP_NAMESPACE::detail::has_subscript_operator<Type, IndexType>
			inline constexpr auto& operator[](const IndexType& index) noexcept { return this->value[index]; }

			/// @brief Const lvalue subscript operator.
			/// @returns const auto&
			template <typename IndexType>
				requires STRICT_CPP_NAMESPACE::detail::has_subscript_operator<Type, IndexType>
			inline constexpr const auto& operator[](const IndexType& index) const noexcept { return this->value[index]; }

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
				if constexpr (STRICT_CPP_NAMESPACE::detail::can_convert_to_string<Type>) return static_cast<std::string>(this->value);
				else return std::to_string(this->value);
			}

			template <typename _ = void>
				requires STRICT_CPP_NAMESPACE::detail::can_wstringify<Type>
			inline std::wstring m_get_wstring_internal() const noexcept {
				if constexpr (STRICT_CPP_NAMESPACE::detail::can_convert_to_wstring<Type>) return static_cast<std::wstring>(this->value);
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

// Defines a strict alias type.
//
// Usage example:
//    STRICT_CPP_DEFINE_ALIAS_TYPE(my_vector_t, std::vector<int>)
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
					requires STRICT_CPP_NAMESPACE::detail::can_wstringify<TYPE>                                                                                                               \
				inline std::wstring to_wstring() const noexcept {                                                                                                                            \
					return this->m_get_wstring_internal();                                                                                                                                    \
				}                                                                                                                                                                            \
                                                                                                                                                                                         \
				template <typename _ = void>                                                                                                                                                 \
					requires (!STRICT_CPP_NAMESPACE::detail::can_stringify<TYPE>)                                                                                                             \
				inline std::string to_string() const noexcept {                                                                                                                              \
					return "strict::" #NAME;                                                                                                                                                  \
				}                                                                                                                                                                            \
                                                                                                                                                                                         \
				template <typename _ = void>                                                                                                                                                 \
					requires (!STRICT_CPP_NAMESPACE::detail::can_wstringify<TYPE>)                                                                                                            \
				inline std::wstring to_wstring() const noexcept {                                                                                                                            \
					return L"strict::" L#NAME;                                                                                                                                                \
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

// Defines a strict dynamic alias type.
//
// Usage example:
//    STRICT_CPP_DEFINE_DYNAMIC_ALIAS_TYPE(my_dynamic_vector, std::vector)
#define STRICT_CPP_DEFINE_DYNAMIC_ALIAS_TYPE(NAME, TYPE)                                                                                                                                          \
	namespace STRICT_CPP_NAMESPACE {                                                                                                                                                               \
		template <typename... Args>                                                                                                                                                                 \
		struct NAME : STRICT_CPP_NAMESPACE::strict_alias_type<TYPE<Args...>> {                                                                                                                      \
				using Type = TYPE<Args...>;                                                                                                                                                           \
				using STRICT_CPP_NAMESPACE::strict_alias_type<TYPE<Args...>>::strict_alias_type;                                                                                                      \
				using STRICT_CPP_NAMESPACE::strict_alias_type<TYPE<Args...>>::operator=;                                                                                                              \
				using STRICT_CPP_NAMESPACE::strict_alias_type<TYPE<Args...>>::operator->;                                                                                                             \
				using STRICT_CPP_NAMESPACE::strict_alias_type<TYPE<Args...>>::operator[];                                                                                                             \
                                                                                                                                                                                                  \
				template <typename _ = void>                                                                                                                                                          \
					requires STRICT_CPP_NAMESPACE::detail::can_stringify<TYPE<Args...>>                                                                                                                \
				inline std::string to_string() const noexcept {                                                                                                                                       \
					return this->m_get_string_internal();                                                                                                                                              \
				}                                                                                                                                                                                     \
                                                                                                                                                                                                  \
				template <typename _ = void>                                                                                                                                                          \
					requires STRICT_CPP_NAMESPACE::detail::can_wstringify<TYPE<Args...>>                                                                                                               \
				inline std::wstring to_wstring() const noexcept {                                                                                                                                     \
					return this->m_get_wstring_internal();                                                                                                                                             \
				}                                                                                                                                                                                     \
                                                                                                                                                                                                  \
				template <typename _ = void>                                                                                                                                                          \
					requires (!STRICT_CPP_NAMESPACE::detail::can_wstringify<TYPE<Args...>>)                                                                                                            \
				inline std::string to_string() const noexcept {                                                                                                                                       \
					return "strict::" #NAME "<" #TYPE "<...>>";                                                                                                                                        \
				}                                                                                                                                                                                     \
                                                                                                                                                                                                  \
				template <typename _ = void>                                                                                                                                                          \
					requires (!STRICT_CPP_NAMESPACE::detail::can_wstringify<TYPE<Args...>>)                                                                                                            \
				inline std::wstring to_wstring() const noexcept {                                                                                                                                     \
					return L"strict::" L#NAME L"<" L#TYPE L"<...>>";                                                                                                                                   \
				}                                                                                                                                                                                     \
		};                                                                                                                                                                                          \
	}                                                                                                                                                                                              \
	template <typename... Args>                                                                                                                                                                    \
	struct _STD formatter<STRICT_CPP_NAMESPACE::NAME<Args...>> {                                                                                                                                   \
			inline constexpr auto parse(const _STD format_parse_context& context) const noexcept { return context.begin(); }                                                                         \
			inline constexpr auto parse(const _STD wformat_parse_context& context) const noexcept { return context.begin(); }                                                                        \
			inline auto				 format(const STRICT_CPP_NAMESPACE::NAME<Args...>& value, _STD format_context& context) const { return _STD format_to(context.out(), "{}", value.to_string()); }    \
			inline auto				 format(const STRICT_CPP_NAMESPACE::NAME<Args...>& value, _STD wformat_context& context) const { return _STD format_to(context.out(), L"{}", value.to_wstring()); } \
	};
