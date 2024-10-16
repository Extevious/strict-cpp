#pragma once

#if !defined(STRICT_CPP_NAMESPACE)
   #define STRICT_CPP_NAMESPACE strict
#endif

#define STRICT_CPP_INLINE    inline
#define STRICT_CPP_CONSTEXPR constexpr
#define STRICT_CPP_NOEXCEPT  noexcept

#include <cmath>
#include <cstdint>
#include <limits>
#include <string>
#include <type_traits>

namespace STRICT_CPP_NAMESPACE {
   namespace detail {
      // Base type for integral or float type qualifications.
      struct strict_cpp_base_t { };

      // Base type for float-only type qualifications.
      struct strict_cpp_float_base_t { };

      // Base type for integer-only type qualifications.
      struct strict_cpp_integral_base_t { };

      template <typename Type>
      concept is_integral = std::is_integral_v<Type>;

      template <typename Type>
      concept is_float = std::is_floating_point_v<Type>;

      template <typename Type>
      concept is_arithmetic = std::is_arithmetic_v<Type>;

      template <typename Type>
      concept is_scalar = std::is_scalar_v<Type>;

      template <typename Left, typename Right>
      concept is_same = std::is_same_v<Left, Right>;

      template <typename Left, typename Right>
      concept is_convertible = std::is_convertible_v<Left, Right>;

      template <typename Derived>
      concept is_strict_base = std::is_base_of_v<strict_cpp_base_t, Derived>;

      template <typename Derived>
      concept is_strict_float_base = std::is_base_of_v<strict_cpp_float_base_t, Derived>;

      template <typename Derived>
      concept is_strict_integral_base = std::is_base_of_v<strict_cpp_integral_base_t, Derived>;

      // Implicit casts from primitive types should always convert to the exact same encapsulated primitive type.
      template <typename From, typename To>
      concept is_qualified_implicit_convertible = is_same<From, To>;

      // Explicit casts are allowed to convert to any convertible type.
      template <typename From, typename To>
      concept is_qualified_explicit_convertible = (is_convertible<From, To> || is_strict_base<To>) && (!is_qualified_implicit_convertible<From, To>);

      template <typename Type, typename Other>
      concept is_qualified_implicit_constructor = is_qualified_implicit_convertible<Other, Type>;

      template <typename Type, typename Other>
      concept is_qualified_explicit_constructor = is_qualified_explicit_convertible<Other, Type>;

      template <typename... Types>
      concept is_qualified_operator = (is_strict_base<Types> && ...);

      template <typename Left, typename Right>
      concept is_qualified_operator_left_only = (is_qualified_operator<Left> && is_scalar<Right>);

      template <typename Left, typename Right>
      concept is_qualified_operator_right_only = (is_scalar<Left> && is_qualified_operator<Right>);

      template <typename... Types>
      concept is_qualified_integral_operator = (is_strict_integral_base<Types> && ...);

      template <typename Left, typename Right>
      concept is_qualified_integral_operator_left_only = (is_qualified_integral_operator<Left> && is_scalar<Right>);

      template <typename Left, typename Right>
      concept is_qualified_integral_operator_right_only = (is_scalar<Left> && is_qualified_integral_operator<Right>);

      template <typename... Types>
      concept is_qualified_float_operator = (is_strict_float_base<Types> && ...);

      template <typename Left, typename Right>
      concept is_qualified_float_operator_left_only = (is_qualified_float_operator<Left> && (is_arithmetic<Right> || is_strict_integral_base<Right>));

      template <typename Left, typename Right>
      concept is_qualified_float_operator_right_only = ((is_arithmetic<Left> || is_strict_integral_base<Left>) && is_qualified_float_operator<Right>);
   }

#define STRICT_CPP_DEFINE_INTEGRAL_OPERATOR(OPERATOR_TYPE)                                                                                                                         \
   template <typename Left, typename Right>                                                                                                                                        \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_integral_operator<Left, Right>                                                                                           \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR Left operator OPERATOR_TYPE(const Left& left, const Right& right) STRICT_CPP_NOEXCEPT {                                                  \
      return static_cast<Left>(left.value OPERATOR_TYPE right.value);                                                                                                              \
   }                                                                                                                                                                               \
   template <typename Left, typename Right>                                                                                                                                        \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_integral_operator_left_only<Left, Right>                                                                                 \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR Left operator OPERATOR_TYPE(const Left& left, const Right& right) STRICT_CPP_NOEXCEPT {                                                  \
      return static_cast<Left>(left.value OPERATOR_TYPE right);                                                                                                                    \
   }                                                                                                                                                                               \
   template <typename Left, typename Right>                                                                                                                                        \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_integral_operator_right_only<Left, Right>                                                                                \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR Left operator OPERATOR_TYPE(const Left& left, const Right& right) STRICT_CPP_NOEXCEPT {                                                  \
      return static_cast<Left>(left OPERATOR_TYPE right.value);                                                                                                                    \
   }

#define STRICT_CPP_DEFINE_FLOAT_OPERATOR(OPERATOR_TYPE)                                                                                                                            \
   template <typename Left, typename Right>                                                                                                                                        \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator<Left, Right>                                                                                              \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR Left operator OPERATOR_TYPE(const Left& left, const Right& right) STRICT_CPP_NOEXCEPT {                                                  \
      return static_cast<Left>(left.value OPERATOR_TYPE right.value);                                                                                                              \
   }                                                                                                                                                                               \
   template <typename Left, typename Right>                                                                                                                                        \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator_left_only<Left, Right>                                                                                    \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR Left operator OPERATOR_TYPE(const Left& left, const Right& right) STRICT_CPP_NOEXCEPT {                                                  \
      return static_cast<Left>(left.value OPERATOR_TYPE right);                                                                                                                    \
   }                                                                                                                                                                               \
   template <typename Left, typename Right>                                                                                                                                        \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator_right_only<Left, Right>                                                                                   \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR Left operator OPERATOR_TYPE(const Left& left, const Right& right) STRICT_CPP_NOEXCEPT {                                                  \
      return static_cast<Left>(left OPERATOR_TYPE right.value);                                                                                                                    \
   }

#define STRICT_CPP_DEFINE_COMPOUND_INTEGRAL_OPERATOR(OPERATOR_TYPE)                                                                                                                \
   template <typename Left, typename Right>                                                                                                                                        \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_integral_operator<Left, Right>                                                                                           \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR Left& operator OPERATOR_TYPE(Left & left, const Right & right) STRICT_CPP_NOEXCEPT {                                                     \
      left.value OPERATOR_TYPE right.value;                                                                                                                                        \
      return left;                                                                                                                                                                 \
   }                                                                                                                                                                               \
   template <typename Left, typename Right>                                                                                                                                        \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_integral_operator_left_only<Left, Right>                                                                                 \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR Left& operator OPERATOR_TYPE(Left & left, const Right & right) STRICT_CPP_NOEXCEPT {                                                     \
      left.value OPERATOR_TYPE right;                                                                                                                                              \
      return left;                                                                                                                                                                 \
   }                                                                                                                                                                               \
   template <typename Left, typename Right>                                                                                                                                        \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_integral_operator_right_only<Left, Right>                                                                                \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR Left& operator OPERATOR_TYPE(Left & left, const Right & right) STRICT_CPP_NOEXCEPT {                                                     \
      left OPERATOR_TYPE right.value;                                                                                                                                              \
      return left;                                                                                                                                                                 \
   }

#define STRICT_CPP_DEFINE_COMPOUND_FLOAT_OPERATOR(OPERATOR_TYPE)                                                                                                                   \
   template <typename Left, typename Right>                                                                                                                                        \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator<Left, Right>                                                                                              \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR Left& operator OPERATOR_TYPE(Left & left, const Right & right) STRICT_CPP_NOEXCEPT {                                                     \
      left.value OPERATOR_TYPE right.value;                                                                                                                                        \
      return left;                                                                                                                                                                 \
   }                                                                                                                                                                               \
   template <typename Left, typename Right>                                                                                                                                        \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator_left_only<Left, Right>                                                                                    \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR Left& operator OPERATOR_TYPE(Left & left, const Right & right) STRICT_CPP_NOEXCEPT {                                                     \
      left.value OPERATOR_TYPE right;                                                                                                                                              \
      return left;                                                                                                                                                                 \
   }                                                                                                                                                                               \
   template <typename Left, typename Right>                                                                                                                                        \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator_right_only<Left, Right>                                                                                   \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR Left& operator OPERATOR_TYPE(Left & left, const Right & right) STRICT_CPP_NOEXCEPT {                                                     \
      left OPERATOR_TYPE right.value;                                                                                                                                              \
      return left;                                                                                                                                                                 \
   }

#define STRICT_CPP_DEFINE_COMPARISON_OPERATOR(OPERATOR_TYPE)                                                                                                                       \
   template <typename Left, typename Right>                                                                                                                                        \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator<Left, Right>                                                                                                    \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR bool operator OPERATOR_TYPE(const Left& left, const Right& right) STRICT_CPP_NOEXCEPT {                                                  \
      return left.value OPERATOR_TYPE right.value;                                                                                                                                 \
   }                                                                                                                                                                               \
   template <typename Left, typename Right>                                                                                                                                        \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator_left_only<Left, Right>                                                                                          \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR bool operator OPERATOR_TYPE(const Left& left, const Right& right) STRICT_CPP_NOEXCEPT {                                                  \
      return left.value OPERATOR_TYPE right;                                                                                                                                       \
   }                                                                                                                                                                               \
   template <typename Left, typename Right>                                                                                                                                        \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator_right_only<Left, Right>                                                                                         \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR bool operator OPERATOR_TYPE(const Left& left, const Right& right) STRICT_CPP_NOEXCEPT {                                                  \
      return left OPERATOR_TYPE right.value;                                                                                                                                       \
   }

   // =============================================================================
   // Operators for both integral and floating-point types
   // =============================================================================

   template <typename Type>
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator<Type>
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR Type operator++(Type& value) STRICT_CPP_NOEXCEPT {
      return static_cast<Type>(++value.value);
   }

   template <typename Type>
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator<Type>
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR Type operator++(Type& value, auto) STRICT_CPP_NOEXCEPT {
      return static_cast<Type>(value.value++);
   }

   template <typename Type>
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator<Type>
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR Type operator--(Type& value) STRICT_CPP_NOEXCEPT {
      return static_cast<Type>(--value.value);
   }

   template <typename Type>
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator<Type>
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR Type operator--(Type& value, auto) STRICT_CPP_NOEXCEPT {
      return static_cast<Type>(value.value--);
   }

   template <typename Type>
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator<Type>
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR Type operator+(const Type& value) STRICT_CPP_NOEXCEPT {
      return static_cast<Type>(+value.value);
   }

   template <typename Type>
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator<Type>
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR Type operator-(const Type& value) STRICT_CPP_NOEXCEPT {
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
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR Type operator~(const Type& value) STRICT_CPP_NOEXCEPT {
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
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR Left operator%(const Left& left, const Right& right) STRICT_CPP_NOEXCEPT {
      if constexpr (__cplusplus >= 202207L) return static_cast<Left>(std::fmod(left.value, right.value));
      else return static_cast<Left>(std::fmodl(left.value, right.value));
   }

   template <typename Left, typename Right>
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator_left_only<Left, Right>
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR Left operator%(const Left& left, const Right& right) STRICT_CPP_NOEXCEPT {
      if constexpr (__cplusplus >= 202207L) return static_cast<Left>(std::fmod(left.value, right));
      else return static_cast<Left>(std::fmodl(left.value, right));
   }

   template <typename Left, typename Right>
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator_right_only<Left, Right>
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR Left operator%(const Left& left, const Right& right) STRICT_CPP_NOEXCEPT {
      if constexpr (__cplusplus >= 202207L) return static_cast<Left>(std::fmod(left, right.value));
      else return static_cast<Left>(std::fmodl(left, right.value));
   }

   template <typename Left, typename Right>
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator<Left, Right>
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR Left& operator%=(Left& left, const Right& right) STRICT_CPP_NOEXCEPT {
      if constexpr (__cplusplus >= 202207L) left.value = static_cast<typename Left::type>(std::fmod(left.value, right.value));
      else left.value = static_cast<typename Left::type>(std::fmodl(left.value, right.value));

      return left;
   }

   template <typename Left, typename Right>
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator_left_only<Left, Right>
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR Left& operator%=(Left& left, const Right& right) STRICT_CPP_NOEXCEPT {
      if constexpr (__cplusplus >= 202207L) left.value = static_cast<typename Left::type>(std::fmod(left.value, right));
      else left.value = static_cast<typename Left::type>(std::fmodl(left.value, right));

      return left;
   }

   template <typename Left, typename Right>
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator_right_only<Left, Right>
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR Left& operator%=(Left& left, const Right& right) STRICT_CPP_NOEXCEPT {
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

   template <typename Type>
   struct strict_type_base : STRICT_CPP_NAMESPACE::detail::strict_cpp_base_t {
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static Type min = std::numeric_limits<Type>::min();
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static Type max = std::numeric_limits<Type>::max();

         using type = Type;
         Type value = {};

         /// @brief Default constructor.
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR strict_type_base() STRICT_CPP_NOEXCEPT = default;

         /// @brief Implicit constructor.
         /// @tparam Other The implicitly-constructable type.
         /// @param other The implicitly-constructable value.
         template <typename Other>
            requires STRICT_CPP_NAMESPACE::detail::is_qualified_implicit_constructor<Type, Other>
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR strict_type_base(const Other& other) STRICT_CPP_NOEXCEPT : value(static_cast<Type>(other)) { }

         /// @brief Explicit constructor.
         /// @tparam Other The explicitly-constructable type.
         /// @param other The explicitly-constructable value.
         template <typename Other>
            requires STRICT_CPP_NAMESPACE::detail::is_qualified_explicit_constructor<Type, Other>
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR explicit strict_type_base(const Other& other) STRICT_CPP_NOEXCEPT : value(static_cast<Type>(other)) { }

         /// @brief Assignment operator.
         /// @tparam Other The assignment type.
         /// @returns auto&
         template <typename Other>
            requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator<Type, Other>
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR auto& operator=(const Other& other) STRICT_CPP_NOEXCEPT {
            this->value = other.value;
            return *this;
         }

         /// @brief Implicit conversion operator converts to the same encapsulated type only.
         /// @returns Type
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR operator Type() const STRICT_CPP_NOEXCEPT { return this->value; }

         /// @brief Explicit conversion operator.
         /// @tparam Other The type to convert to.
         /// @returns Other
         template <typename Other>
            requires STRICT_CPP_NAMESPACE::detail::is_convertible<Type, Other>
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR explicit operator Other() const STRICT_CPP_NOEXCEPT {
            return static_cast<Other>(this->value);
         }

         /// @brief Conversion function.
         /// @tparam Other The type to convert to.
         /// @returns Other
         template <typename Other>
            requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator<Other>
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR Other as() const STRICT_CPP_NOEXCEPT {
            return static_cast<Other>(this->value);
         }

         /// @brief Converts to a human-readable string representing the current value.
         /// @returns std::string
         STRICT_CPP_INLINE std::string to_string() const { return std::to_string(value); }

         /// @brief Converts to a human-readable wide string representing the current value.
         /// @returns std::wstring
         STRICT_CPP_INLINE std::wstring to_wstring() const { return std::to_wstring(value); }
   };
}

#define STRICT_CPP_DEFINE_INTEGRAL_TYPE(STRICT_CPP_TYPE, TYPE)                                                                                                                     \
   struct STRICT_CPP_TYPE : STRICT_CPP_NAMESPACE::strict_type_base<TYPE>, STRICT_CPP_NAMESPACE::detail::strict_cpp_integral_base_t {                                               \
         using STRICT_CPP_NAMESPACE::strict_type_base<TYPE>::strict_type_base;                                                                                                     \
   };

#define STRICT_CPP_DEFINE_FLOAT_TYPE(STRICT_CPP_TYPE, TYPE)                                                                                                                        \
   struct STRICT_CPP_TYPE : STRICT_CPP_NAMESPACE::strict_type_base<TYPE>, STRICT_CPP_NAMESPACE::detail::strict_cpp_float_base_t {                                                  \
         using STRICT_CPP_NAMESPACE::strict_type_base<TYPE>::strict_type_base;                                                                                                     \
                                                                                                                                                                                   \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static TYPE min         = std::numeric_limits<TYPE>::min();                                                                        \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static TYPE max         = std::numeric_limits<TYPE>::max();                                                                        \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static TYPE lowest      = std::numeric_limits<TYPE>::lowest();                                                                     \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static TYPE qNaN        = std::numeric_limits<TYPE>::quiet_NaN();                                                                  \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static TYPE sNaN        = std::numeric_limits<TYPE>::signaling_NaN();                                                              \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static TYPE posInfinity = std::numeric_limits<TYPE>::infinity();                                                                   \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static TYPE negInfinity = -std::numeric_limits<TYPE>::infinity();                                                                  \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static TYPE epsilon     = std::numeric_limits<TYPE>::epsilon();                                                                    \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static TYPE roundError  = std::numeric_limits<TYPE>::round_error();                                                                \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static TYPE denormal    = std::numeric_limits<TYPE>::denorm_min();                                                                 \
   };

#pragma warning(disable : 4146)

// =============================================================================
// Pre-defined types
// =============================================================================

namespace STRICT_CPP_NAMESPACE {
   // Common integral types:
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(size_t, std::size_t);

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

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(char_t, char);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(schar_t, signed char);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(uchar_t, unsigned char);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(byte_t, unsigned char);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(sbyte_t, signed char);

   // Common floating-point types:
   STRICT_CPP_DEFINE_FLOAT_TYPE(float_t, float);
   STRICT_CPP_DEFINE_FLOAT_TYPE(double_t, double);
   STRICT_CPP_DEFINE_FLOAT_TYPE(long_double_t, long double);

#if defined(STRICT_CPP_OPTIONAL_TYPES)
   // Optional integral types:
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(size8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(size16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(size32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(size64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(offset_size_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(offset_size8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(offset_size16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(offset_size32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(offset_size64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(src_size_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(src_size8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(src_size16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(src_size32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(src_size64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(src_offset_size_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(src_offset_size8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(src_offset_size16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(src_offset_size32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(src_offset_size64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(begin_size_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(begin_size8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(begin_size16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(begin_size32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(begin_size64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(begin_offset_size_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(begin_offset_size8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(begin_offset_size16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(begin_offset_size32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(begin_offset_size64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(lhs_size_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(lhs_size8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(lhs_size16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(lhs_size32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(lhs_size64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(lhs_offset_size_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(lhs_offset_size8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(lhs_offset_size16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(lhs_offset_size32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(lhs_offset_size64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(dst_size_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(dst_size8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(dst_size16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(dst_size32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(dst_size64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(dst_offset_size_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(dst_offset_size8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(dst_offset_size16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(dst_offset_size32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(dst_offset_size64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(end_size_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(end_size8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(end_size16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(end_size32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(end_size64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(end_offset_size_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(end_offset_size8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(end_offset_size16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(end_offset_size32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(end_offset_size64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(rhs_size_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(rhs_size8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(rhs_size16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(rhs_size32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(rhs_size64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(rhs_offset_size_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(rhs_offset_size8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(rhs_offset_size16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(rhs_offset_size32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(rhs_offset_size64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(count_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(count8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(count16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(count32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(count64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(offset_count_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(offset_count8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(offset_count16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(offset_count32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(offset_count64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(src_count_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(src_count8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(src_count16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(src_count32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(src_count64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(src_offset_count_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(src_offset_count8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(src_offset_count16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(src_offset_count32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(src_offset_count64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(begin_count_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(begin_count8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(begin_count16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(begin_count32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(begin_count64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(begin_offset_count_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(begin_offset_count8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(begin_offset_count16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(begin_offset_count32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(begin_offset_count64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(lhs_count_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(lhs_count8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(lhs_count16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(lhs_count32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(lhs_count64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(lhs_offset_count_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(lhs_offset_count8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(lhs_offset_count16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(lhs_offset_count32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(lhs_offset_count64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(dst_count_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(dst_count8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(dst_count16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(dst_count32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(dst_count64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(dst_offset_count_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(dst_offset_count8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(dst_offset_count16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(dst_offset_count32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(dst_offset_count64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(end_count_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(end_count8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(end_count16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(end_count32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(end_count64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(end_offset_count_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(end_offset_count8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(end_offset_count16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(end_offset_count32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(end_offset_count64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(rhs_count_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(rhs_count8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(rhs_count16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(rhs_count32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(rhs_count64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(rhs_offset_count_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(rhs_offset_count8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(rhs_offset_count16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(rhs_offset_count32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(rhs_offset_count64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(offset_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(offset8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(offset16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(offset32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(offset64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(src_offset_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(src_offset8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(src_offset16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(src_offset32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(src_offset64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(begin_offset_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(begin_offset8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(begin_offset16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(begin_offset32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(begin_offset64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(dst_offset_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(dst_offset8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(dst_offset16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(dst_offset32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(dst_offset64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(end_offset_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(end_offset8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(end_offset16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(end_offset32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(end_offset64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(capacity_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(capacity8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(capacity16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(capacity32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(capacity64_t, std::uint64_t);

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(index_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(index8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(index16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(index32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(index64_t, std::uint64_t);
#endif
}
