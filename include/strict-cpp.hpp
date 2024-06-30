#pragma once

#define STRICT_CPP_STR(...)  #__VA_ARGS__
#define STRICT_CPP_XSTR(...) STRICT_CPP_STR(__VA_ARGS__)

#define STRICT_CPP_JOIN_STR(a, b)  a##b
#define STRICT_CPP_JOIN_XSTR(a, b) STRICT_CPP_JOIN_STR(a, b)

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
      // Base type for integral<->float type qualifications.
      struct strict_cpp_base_t { };

      // Base type for float-only type qualifications.
      struct strict_cpp_float_base_t : public strict_cpp_base_t { };

      // Base type for int-only type qualifications.
      struct strict_cpp_integral_base_t : public strict_cpp_base_t { };

      template <typename Left, typename Right>
      concept is_type_compliant = std::is_same_v<Left, Right>;

      template <typename Derived>
      concept is_base_compliant = std::is_base_of_v<strict_cpp_base_t, Derived>;

      template <typename T>
      concept is_scalar_compliant = std::is_scalar_v<T>;

      template <typename T>
      concept is_float_compliant = std::is_floating_point_v<T>;

      template <typename Derived>
      concept is_float_base_compliant = std::is_base_of_v<strict_cpp_float_base_t, Derived>;

      template <typename Derived>
      concept is_integral_base_compliant = std::is_base_of_v<strict_cpp_integral_base_t, Derived>;

      template <typename Left, typename Right>
      concept is_conversion_compliant = std::is_convertible_v<Left, Right>;

      // Implicit casts should always convert to the exact same type.
      template <typename Left, typename Right>
      concept is_implicit_conversion_compliant = is_conversion_compliant<Left, Right> && is_type_compliant<Left, Right>;

      // Explicit casts are allowed to convert to any convertible type.
      template <typename Left, typename Right>
      concept is_explicit_conversion_compliant = is_conversion_compliant<Left, Right> && !is_implicit_conversion_compliant<Left, Right>;

      template <typename Left, typename Right>
      concept is_qualified_type = is_type_compliant<Left, Right>;

      template <typename Left, typename Right>
      concept is_qualified_implicit_constructor = is_qualified_type<Left, Right>;

      template <typename Derived, typename T, typename Other>
      concept is_qualified_explicit_constructor = (is_explicit_conversion_compliant<T, Other> || is_base_compliant<Derived>)&&(!is_qualified_implicit_constructor<T, Other>);

      template <typename... Types>
      concept is_qualified_operator = (is_base_compliant<Types> && ...);

      template <typename Left, typename Right>
      concept is_qualified_operator_left_only = (is_qualified_operator<Left> && is_scalar_compliant<Right>);

      template <typename Left, typename Right>
      concept is_qualified_operator_right_only = (is_qualified_operator<Right> && is_scalar_compliant<Left>);

      template <typename... Types>
      concept is_qualified_integral_operator = (is_integral_base_compliant<Types> && ...);

      template <typename Left, typename Right>
      concept is_qualified_integral_operator_left_only = (is_qualified_integral_operator<Left> && is_scalar_compliant<Right> && !is_float_compliant<Right>);

      template <typename Left, typename Right>
      concept is_qualified_integral_operator_right_only = (is_qualified_integral_operator<Right> && is_scalar_compliant<Left> && !is_float_compliant<Left>);

      template <typename... Types>
      concept is_qualified_float_operator = (is_float_base_compliant<Types> && ...);

      template <typename Left, typename Right>
      concept is_qualified_float_operator_left_only = (is_qualified_float_operator<Left> && (std::is_arithmetic_v<Right> || is_qualified_integral_operator<Right>));

      template <typename Left, typename Right>
      concept is_qualified_float_operator_right_only = (is_qualified_float_operator<Right> && (std::is_arithmetic_v<Left> || is_qualified_integral_operator<Left>));
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

   // =============================================================================
   // Operators for both integral and floating-point types
   // =============================================================================

   template <typename T>
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator<T>
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR T operator++(T& value) STRICT_CPP_NOEXCEPT {
      return static_cast<T>(++value.value);
   }

   template <typename T>
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator<T>
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR T operator++(T& value, auto) STRICT_CPP_NOEXCEPT {
      return static_cast<T>(value.value++);
   }

   template <typename T>
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator<T>
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR T operator--(T& value) STRICT_CPP_NOEXCEPT {
      return static_cast<T>(--value.value);
   }

   template <typename T>
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator<T>
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR T operator--(T& value, auto) STRICT_CPP_NOEXCEPT {
      return static_cast<T>(value.value++);
   }

   template <typename Left, typename Right>
      requires strict::detail::is_qualified_operator<Left, Right>
   inline constexpr auto operator<=>(const Left& left, const Right& right) noexcept {
      return left.value <=> right.value;
   }

   template <typename Left, typename Right>
      requires strict::detail::is_qualified_operator_left_only<Left, Right>
   inline constexpr auto operator<=>(const Left& left, const Right& right) noexcept {
      return left.value <=> right;
   }

   template <typename Left, typename Right>
      requires strict::detail::is_qualified_operator_right_only<Left, Right>
   inline constexpr auto operator<=>(const Left& left, const Right& right) noexcept {
      return left <=> right.value;
   }

   // ==========================================================================
   // Integral-only operators
   // ==========================================================================

   template <typename T>
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_integral_operator<T>
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR T operator~(const T& value) STRICT_CPP_NOEXCEPT {
      return static_cast<T>(~value.value);
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
}

#define STRICT_CPP_DEFINE_INTEGRAL_TYPE(STRICT_CPP_TYPE, T)                                                                                                                        \
   struct STRICT_CPP_TYPE : STRICT_CPP_NAMESPACE::detail::strict_cpp_integral_base_t {                                                                                             \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static T min = std::numeric_limits<T>::min();                                                                                      \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static T max = std::numeric_limits<T>::max();                                                                                      \
                                                                                                                                                                                   \
         using type = T;                                                                                                                                                           \
         T value    = {};                                                                                                                                                          \
                                                                                                                                                                                   \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR STRICT_CPP_TYPE() STRICT_CPP_NOEXCEPT = default;                                                                                   \
                                                                                                                                                                                   \
         template <typename Other>                                                                                                                                                 \
            requires STRICT_CPP_NAMESPACE::detail::is_qualified_implicit_constructor<T, Other>                                                                                     \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR STRICT_CPP_TYPE(const Other& other) STRICT_CPP_NOEXCEPT : value(static_cast<T>(other)) { }                                         \
                                                                                                                                                                                   \
         template <typename Other>                                                                                                                                                 \
            requires STRICT_CPP_NAMESPACE::detail::is_qualified_explicit_constructor<STRICT_CPP_TYPE, T, Other>                                                                    \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR explicit STRICT_CPP_TYPE(const Other& other) STRICT_CPP_NOEXCEPT : value(static_cast<T>(other)) { }                                \
                                                                                                                                                                                   \
         template <typename Other>                                                                                                                                                 \
            requires STRICT_CPP_NAMESPACE::detail::is_qualified_integral_operator<STRICT_CPP_TYPE, Other>                                                                          \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR STRICT_CPP_TYPE& operator=(const Other& other) STRICT_CPP_NOEXCEPT {                                                               \
            this->value = other.value;                                                                                                                                             \
            return *this;                                                                                                                                                          \
         }                                                                                                                                                                         \
                                                                                                                                                                                   \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR operator T() const STRICT_CPP_NOEXCEPT { return this->value; }                                                                     \
         STRICT_CPP_INLINE explicit             operator std::string() const { return this->to_string(); }                                                                         \
         STRICT_CPP_INLINE explicit             operator std::wstring() const { return this->to_wstring(); }                                                                       \
         STRICT_CPP_INLINE std::string          to_string() const { return std::to_string(value); }                                                                                \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR std::string to_string_t() const STRICT_CPP_NOEXCEPT { return #T; }                                                                 \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR std::string to_string_n() const STRICT_CPP_NOEXCEPT { return STRICT_CPP_XSTR(STRICT_CPP_NAMESPACE) "::" #STRICT_CPP_TYPE; }        \
         STRICT_CPP_INLINE std::wstring         to_wstring() const { return std::to_wstring(value); }                                                                              \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR std::wstring to_wstring_t() const STRICT_CPP_NOEXCEPT { return L#T; }                                                              \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR std::wstring to_wstring_n() const STRICT_CPP_NOEXCEPT {                                                                            \
            return STRICT_CPP_JOIN_XSTR(L, STRICT_CPP_XSTR(STRICT_CPP_NAMESPACE)) L"::" L#STRICT_CPP_TYPE;                                                                         \
         }                                                                                                                                                                         \
   };

#define STRICT_CPP_DEFINE_FLOAT_TYPE(STRICT_CPP_TYPE, T)                                                                                                                           \
   struct STRICT_CPP_TYPE : STRICT_CPP_NAMESPACE::detail::strict_cpp_float_base_t {                                                                                                \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static T min         = std::numeric_limits<T>::min();                                                                              \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static T max         = std::numeric_limits<T>::max();                                                                              \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static T lowest      = std::numeric_limits<T>::lowest();                                                                           \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static T qNaN        = std::numeric_limits<T>::quiet_NaN();                                                                        \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static T sNaN        = std::numeric_limits<T>::signaling_NaN();                                                                    \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static T posInfinity = std::numeric_limits<T>::infinity();                                                                         \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static T negInfinity = -std::numeric_limits<T>::infinity();                                                                        \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static T epsilon     = std::numeric_limits<T>::epsilon();                                                                          \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static T roundError  = std::numeric_limits<T>::round_error();                                                                      \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static T denormal    = std::numeric_limits<T>::denorm_min();                                                                       \
                                                                                                                                                                                   \
         using type = T;                                                                                                                                                           \
         T value    = {};                                                                                                                                                          \
                                                                                                                                                                                   \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR STRICT_CPP_TYPE() STRICT_CPP_NOEXCEPT = default;                                                                                   \
                                                                                                                                                                                   \
         template <typename Other>                                                                                                                                                 \
            requires STRICT_CPP_NAMESPACE::detail::is_qualified_implicit_constructor<T, Other>                                                                                     \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR STRICT_CPP_TYPE(const Other& other) STRICT_CPP_NOEXCEPT : value(static_cast<T>(other)) { }                                         \
                                                                                                                                                                                   \
         template <typename Other>                                                                                                                                                 \
            requires STRICT_CPP_NAMESPACE::detail::is_qualified_explicit_constructor<STRICT_CPP_TYPE, T, Other>                                                                    \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR explicit STRICT_CPP_TYPE(const Other& other) STRICT_CPP_NOEXCEPT : value(static_cast<T>(other)) { }                                \
                                                                                                                                                                                   \
         template <typename Other>                                                                                                                                                 \
            requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator<STRICT_CPP_TYPE, Other>                                                                             \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR STRICT_CPP_TYPE& operator=(const Other& other) STRICT_CPP_NOEXCEPT {                                                               \
            this->value = other.value;                                                                                                                                             \
            return *this;                                                                                                                                                          \
         }                                                                                                                                                                         \
                                                                                                                                                                                   \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR operator T() const STRICT_CPP_NOEXCEPT { return this->value; }                                                                     \
         STRICT_CPP_INLINE explicit             operator std::string() const { return this->to_string(); }                                                                         \
         STRICT_CPP_INLINE explicit             operator std::wstring() const { return this->to_wstring(); }                                                                       \
         STRICT_CPP_INLINE std::string          to_string() const { return std::to_string(value); }                                                                                \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR std::string to_string_t() const STRICT_CPP_NOEXCEPT { return #T; }                                                                 \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR std::string to_string_n() const STRICT_CPP_NOEXCEPT { return STRICT_CPP_XSTR(STRICT_CPP_NAMESPACE) "::" #STRICT_CPP_TYPE; }        \
         STRICT_CPP_INLINE std::wstring         to_wstring() const { return std::to_wstring(value); }                                                                              \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR std::wstring to_wstring_t() const STRICT_CPP_NOEXCEPT { return L#T; }                                                              \
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR std::wstring to_wstring_n() const STRICT_CPP_NOEXCEPT {                                                                            \
            return STRICT_CPP_JOIN_XSTR(L, STRICT_CPP_XSTR(STRICT_CPP_NAMESPACE)) L"::" L#STRICT_CPP_TYPE;                                                                         \
         }                                                                                                                                                                         \
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
