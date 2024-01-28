#pragma once

#include <cstdint>
#if !defined(STRICT_CPP_NAMESPACE)
   #define STRICT_CPP_NAMESPACE strict
#endif

#if !defined(STRICT_CPP_NAMESPACE_STR)
   #define STRICT_CPP_NAMESPACE_STR "strict"
#endif

#if !defined(STRICT_CPP_NAMESPACE_WSTR)
   #define STRICT_CPP_NAMESPACE_WSTR L"strict"
#endif

#define STRICT_CPP_INLINE    inline
#define STRICT_CPP_CONSTEXPR constexpr
#define STRICT_CPP_NOEXCEPT  noexcept

#include <cmath>
#include <limits>
#include <string>
#include <type_traits>

namespace STRICT_CPP_NAMESPACE::detail {
   // Base type for integral<->float type qualifications.
   struct strict_cpp_base_t {
         STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR auto operator<=>(const strict_cpp_base_t&) const STRICT_CPP_NOEXCEPT = default;
   };

   template <typename T1, typename T2>
   concept is_type_compliant = std::is_same_v<T1, T2>;

   template <typename Derived>
   concept is_base_compliant = std::is_base_of_v<strict_cpp_base_t, Derived>;

   template <typename T1, typename T2>
   concept is_conversion_compliant = std::is_convertible_v<T1, T2>;

   // Implicit casts should always convert to the exact same type.
   template <typename T1, typename T2>
   concept is_implicit_conversion_compliant = is_conversion_compliant<T1, T2> && is_type_compliant<T1, T2>;

   // Explicit casts are allowed to convert to any convertible type.
   template <typename T1, typename T2>
   concept is_explicit_conversion_compliant = is_conversion_compliant<T1, T2> && !is_implicit_conversion_compliant<T1, T2>;

   template <typename T1, typename T2>
   concept is_qualified_type = is_type_compliant<T1, T2>;

   template <typename Base, typename T1, typename T2>
   concept is_qualified_constructor = is_qualified_type<T1, T2>;

   template <typename Base, typename T1, typename T2>
   concept is_qualified_explicit_constructor = (is_explicit_conversion_compliant<T1, T2> || is_base_compliant<Base>)&&!is_qualified_constructor<Base, T1, T2>;

   template <typename T1, typename T2>
   concept is_qualified_implicit_cast_operator = is_implicit_conversion_compliant<T1, T2>;

   template <typename T1, typename T2>
   concept is_qualified_explicit_cast_operator = is_explicit_conversion_compliant<T1, T2>;

   template <typename Base, typename T1, typename T2>
   concept is_qualified_operator = is_type_compliant<Base, T2> || is_type_compliant<T1, T2>;

   template <typename T1, typename T2>
      requires std::is_base_of_v<detail::strict_cpp_base_t, T1>
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR auto get_implicit_promoted_or_demoted_value(auto value) STRICT_CPP_NOEXCEPT {
      if constexpr (std::is_base_of_v<detail::strict_cpp_base_t, T2>) {
         if constexpr (std::is_same_v<decltype(value), typename T1::type>) return T1(value);
         else return T2(value);

      } else {
         if constexpr (std::is_same_v<decltype(value), T1>) return T1(value);
         else return T2(value);
      }
   }

}

// The min and max constants of the type
#define DETAIL_STRICT_CPP_MIN_MAX_CONSTANTS(T)                                                                                                                                     \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static T min = std::numeric_limits<T>::min();                                                                                            \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static T max = std::numeric_limits<T>::max();

// Float-only constants
#define DEFINE_STRICT_CPP_FLOAT_CONSTANTS(T)                                                                                                                                       \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static T lowest      = std::numeric_limits<T>::lowest();                                                                                 \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static T qNaN        = std::numeric_limits<T>::quiet_NaN();                                                                              \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static T sNaN        = std::numeric_limits<T>::signaling_NaN();                                                                          \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static T posInfinity = std::numeric_limits<T>::infinity();                                                                               \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static T negInfinity = -std::numeric_limits<T>::infinity();                                                                              \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static T epsilon     = std::numeric_limits<T>::epsilon();                                                                                \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static T roundError  = std::numeric_limits<T>::round_error();                                                                            \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR static T denormal    = std::numeric_limits<T>::denorm_min();

// Constructors
#define DETAIL_STRICT_CPP_CONSTRUCTORS(STRICT_CPP_TYPE, T)                                                                                                                         \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR STRICT_CPP_TYPE() STRICT_CPP_NOEXCEPT = default;                                                                                         \
                                                                                                                                                                                   \
   template <typename Other>                                                                                                                                                       \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_constructor<STRICT_CPP_TYPE, T, Other>                                                                                   \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR STRICT_CPP_TYPE(const Other& other) STRICT_CPP_NOEXCEPT : value(static_cast<T>(other)) { }                                               \
                                                                                                                                                                                   \
   template <typename Other>                                                                                                                                                       \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_explicit_constructor<STRICT_CPP_TYPE, T, Other>                                                                          \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR explicit STRICT_CPP_TYPE(const Other& other) STRICT_CPP_NOEXCEPT : value(static_cast<T>(other)) { }

// Operators for both integral and floating-point types
#define DETAIL_STRICT_CPP_OPERATORS(STRICT_CPP_TYPE, T)                                                                                                                            \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR operator T() const STRICT_CPP_NOEXCEPT { return this->value; }                                                                           \
                                                                                                                                                                                   \
   template <typename Other>                                                                                                                                                       \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_explicit_cast_operator<T, Other>                                                                                         \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR explicit operator Other() const STRICT_CPP_NOEXCEPT {                                                                                    \
      return (Other)this->value;                                                                                                                                                   \
   }                                                                                                                                                                               \
                                                                                                                                                                                   \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR auto operator<=>(const auto& other) const STRICT_CPP_NOEXCEPT { return this->value <=> other; }                                          \
                                                                                                                                                                                   \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE operator+() const STRICT_CPP_NOEXCEPT { return STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE(+value); }    \
                                                                                                                                                                                   \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE operator-() const STRICT_CPP_NOEXCEPT { return STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE(-value); }    \
                                                                                                                                                                                   \
   template <typename Other>                                                                                                                                                       \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR auto operator+(const Other& other) const STRICT_CPP_NOEXCEPT {                                                                           \
      return STRICT_CPP_NAMESPACE::detail::get_implicit_promoted_or_demoted_value<STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE, Other>(this->value + other);                              \
   }                                                                                                                                                                               \
                                                                                                                                                                                   \
   template <typename Other>                                                                                                                                                       \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR auto operator-(const Other& other) const STRICT_CPP_NOEXCEPT {                                                                           \
      return STRICT_CPP_NAMESPACE::detail::get_implicit_promoted_or_demoted_value<STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE, Other>(this->value - other);                              \
   }                                                                                                                                                                               \
                                                                                                                                                                                   \
   template <typename Other>                                                                                                                                                       \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR auto operator*(const Other& other) const STRICT_CPP_NOEXCEPT {                                                                           \
      return STRICT_CPP_NAMESPACE::detail::get_implicit_promoted_or_demoted_value<STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE, Other>(this->value * other);                              \
   }                                                                                                                                                                               \
                                                                                                                                                                                   \
   template <typename Other>                                                                                                                                                       \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR auto operator/(const Other& other) const STRICT_CPP_NOEXCEPT {                                                                           \
      return STRICT_CPP_NAMESPACE::detail::get_implicit_promoted_or_demoted_value<STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE, Other>(this->value / other);                              \
   }                                                                                                                                                                               \
                                                                                                                                                                                   \
   template <typename Other>                                                                                                                                                       \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE& operator+=(const Other& other) STRICT_CPP_NOEXCEPT {                                              \
      this->value += static_cast<T>(other);                                                                                                                                        \
      return *this;                                                                                                                                                                \
   }                                                                                                                                                                               \
                                                                                                                                                                                   \
   template <typename Other>                                                                                                                                                       \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE& operator-=(const Other& other) STRICT_CPP_NOEXCEPT {                                              \
      this->value -= static_cast<T>(other);                                                                                                                                        \
      return *this;                                                                                                                                                                \
   }                                                                                                                                                                               \
                                                                                                                                                                                   \
   template <typename Other>                                                                                                                                                       \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE& operator*=(const Other& other) STRICT_CPP_NOEXCEPT {                                              \
      this->value *= static_cast<T>(other);                                                                                                                                        \
      return *this;                                                                                                                                                                \
   }                                                                                                                                                                               \
                                                                                                                                                                                   \
   template <typename Other>                                                                                                                                                       \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE& operator/=(const Other& other) STRICT_CPP_NOEXCEPT {                                              \
      this->value /= static_cast<T>(other);                                                                                                                                        \
      return *this;                                                                                                                                                                \
   }                                                                                                                                                                               \
                                                                                                                                                                                   \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE operator++() STRICT_CPP_NOEXCEPT { return STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE(++value); }        \
                                                                                                                                                                                   \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE operator++(auto) STRICT_CPP_NOEXCEPT { return STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE(value++); }    \
                                                                                                                                                                                   \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE operator--() STRICT_CPP_NOEXCEPT { return STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE(--value); }        \
                                                                                                                                                                                   \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE operator--(auto) STRICT_CPP_NOEXCEPT { return STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE(value++); }

// Integral-only operators
#define DETAIL_STRICT_CPP_INTEGRAL_OPERATORS(STRICT_CPP_TYPE, T)                                                                                                                   \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE operator~() const STRICT_CPP_NOEXCEPT {                                                            \
      return STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE(~this->value);                                                                                                                  \
   }                                                                                                                                                                               \
                                                                                                                                                                                   \
   template <typename Other>                                                                                                                                                       \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR auto operator&(const Other& other) const STRICT_CPP_NOEXCEPT {                                                                           \
      return STRICT_CPP_NAMESPACE::detail::get_implicit_promoted_or_demoted_value<STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE, Other>(this->value & other);                              \
   }                                                                                                                                                                               \
                                                                                                                                                                                   \
   template <typename Other>                                                                                                                                                       \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR auto operator%(const Other& other) const STRICT_CPP_NOEXCEPT {                                                                           \
      return STRICT_CPP_NAMESPACE::detail::get_implicit_promoted_or_demoted_value<STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE, Other>(this->value % other);                              \
   }                                                                                                                                                                               \
                                                                                                                                                                                   \
   template <typename Other>                                                                                                                                                       \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR auto operator|(const Other& other) const STRICT_CPP_NOEXCEPT {                                                                           \
      return STRICT_CPP_NAMESPACE::detail::get_implicit_promoted_or_demoted_value<STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE, Other>(this->value | other);                              \
   }                                                                                                                                                                               \
                                                                                                                                                                                   \
   template <typename Other>                                                                                                                                                       \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR auto operator^(const Other& other) const STRICT_CPP_NOEXCEPT {                                                                           \
      return STRICT_CPP_NAMESPACE::detail::get_implicit_promoted_or_demoted_value<STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE, Other>(this->value ^ other);                              \
   }                                                                                                                                                                               \
                                                                                                                                                                                   \
   template <typename Other>                                                                                                                                                       \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR auto operator<<(const Other& other) const STRICT_CPP_NOEXCEPT {                                                                          \
      return STRICT_CPP_NAMESPACE::detail::get_implicit_promoted_or_demoted_value<STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE, Other>(this->value << other);                             \
   }                                                                                                                                                                               \
                                                                                                                                                                                   \
   template <typename Other>                                                                                                                                                       \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR auto operator>>(const Other& other) const STRICT_CPP_NOEXCEPT {                                                                          \
      return STRICT_CPP_NAMESPACE::detail::get_implicit_promoted_or_demoted_value<STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE, Other>(this->value >> other);                             \
   }                                                                                                                                                                               \
                                                                                                                                                                                   \
   template <typename Other>                                                                                                                                                       \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE& operator%=(const Other& other) STRICT_CPP_NOEXCEPT {                                              \
      this->value %= static_cast<T>(other);                                                                                                                                        \
      return *this;                                                                                                                                                                \
   }                                                                                                                                                                               \
                                                                                                                                                                                   \
   template <typename Other>                                                                                                                                                       \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE& operator&=(const Other& other) STRICT_CPP_NOEXCEPT {                                              \
      this->value &= static_cast<T>(other);                                                                                                                                        \
      return *this;                                                                                                                                                                \
   }                                                                                                                                                                               \
                                                                                                                                                                                   \
   template <typename Other>                                                                                                                                                       \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE& operator|=(const Other& other) STRICT_CPP_NOEXCEPT {                                              \
      this->value |= static_cast<T>(other);                                                                                                                                        \
      return *this;                                                                                                                                                                \
   }                                                                                                                                                                               \
                                                                                                                                                                                   \
   template <typename Other>                                                                                                                                                       \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE& operator^=(const Other& other) STRICT_CPP_NOEXCEPT {                                              \
      this->value ^= static_cast<T>(other);                                                                                                                                        \
      return *this;                                                                                                                                                                \
   }                                                                                                                                                                               \
                                                                                                                                                                                   \
   template <typename Other>                                                                                                                                                       \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE& operator<<=(const Other& other) STRICT_CPP_NOEXCEPT {                                             \
      this->value <<= static_cast<T>(other);                                                                                                                                       \
      return *this;                                                                                                                                                                \
   }                                                                                                                                                                               \
                                                                                                                                                                                   \
   template <typename Other>                                                                                                                                                       \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE& operator>>=(const Other& other) STRICT_CPP_NOEXCEPT {                                             \
      this->value >>= static_cast<T>(other);                                                                                                                                       \
      return *this;                                                                                                                                                                \
   }

// Float-only operators
#if __cplusplus >= 202207L
   #define DEFINE_STRICT_CPP_FLOAT_OPERATORS(STRICT_CPP_TYPE, T)                                                                                                                   \
      template <typename Other>                                                                                                                                                    \
      STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR Other& operator%(const Other& other) STRICT_CPP_NOEXCEPT {                                                                            \
         return STRICT_CPP_NAMESPACE::detail::get_implicit_promoted_or_demoted_value<STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE, Other>(std::fmod(this->value, other));                 \
      }                                                                                                                                                                            \
                                                                                                                                                                                   \
      template <typename Other>                                                                                                                                                    \
      STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE& operator%=(const Other& other) STRICT_CPP_NOEXCEPT {                                           \
         this->value = std::fmod(this->value, static_cast<T>(other));                                                                                                              \
         return *this;                                                                                                                                                             \
      }
#else
   #define DEFINE_STRICT_CPP_FLOAT_OPERATORS(STRICT_CPP_TYPE, T)                                                                                                                   \
      template <typename Other>                                                                                                                                                    \
      STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR Other operator%(const Other& other) STRICT_CPP_NOEXCEPT {                                                                             \
         return STRICT_CPP_NAMESPACE::detail::get_implicit_promoted_or_demoted_value<STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE, Other>(std::fmodl(this->value, other));                \
      }                                                                                                                                                                            \
                                                                                                                                                                                   \
      template <typename Other>                                                                                                                                                    \
      STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR STRICT_CPP_NAMESPACE::STRICT_CPP_TYPE& operator%=(const Other& other) STRICT_CPP_NOEXCEPT {                                           \
         this->value = std::fmodl(this->value, static_cast<T>(other));                                                                                                             \
         return *this;                                                                                                                                                             \
      }
#endif

#define DETAIL_STRICT_CPP_STRINGS(STRICT_CPP_TYPE, T)                                                                                                                              \
   STRICT_CPP_INLINE operator std::string() const { return to_string(); }                                                                                                          \
                                                                                                                                                                                   \
   STRICT_CPP_INLINE std::string to_string() const { return std::to_string(value); }                                                                                               \
                                                                                                                                                                                   \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR std::string to_string_t() const STRICT_CPP_NOEXCEPT { return "" #T; }                                                                    \
                                                                                                                                                                                   \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR std::string to_string_n() const STRICT_CPP_NOEXCEPT { return STRICT_CPP_NAMESPACE_STR "::" #STRICT_CPP_TYPE; }                           \
                                                                                                                                                                                   \
   STRICT_CPP_INLINE std::wstring to_wstring() const { return std::to_wstring(value); }                                                                                            \
                                                                                                                                                                                   \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR std::wstring to_wstring_t() const STRICT_CPP_NOEXCEPT { return L"" L#T; }                                                                \
                                                                                                                                                                                   \
   STRICT_CPP_INLINE STRICT_CPP_CONSTEXPR std::wstring to_wstring_n() const STRICT_CPP_NOEXCEPT { return STRICT_CPP_NAMESPACE_WSTR L"::" L#STRICT_CPP_TYPE; }

#define STRICT_CPP_DEFINE_INTEGRAL_TYPE(STRICT_CPP_TYPE, T)                                                                                                                        \
   struct STRICT_CPP_TYPE : STRICT_CPP_NAMESPACE::detail::strict_cpp_base_t {                                                                                                      \
         using type = T;                                                                                                                                                           \
         T value    = {};                                                                                                                                                          \
                                                                                                                                                                                   \
         DETAIL_STRICT_CPP_MIN_MAX_CONSTANTS(T)                                                                                                                                    \
         DETAIL_STRICT_CPP_CONSTRUCTORS(STRICT_CPP_TYPE, T)                                                                                                                        \
         DETAIL_STRICT_CPP_OPERATORS(STRICT_CPP_TYPE, T)                                                                                                                           \
         DETAIL_STRICT_CPP_INTEGRAL_OPERATORS(STRICT_CPP_TYPE, T)                                                                                                                  \
         DETAIL_STRICT_CPP_STRINGS(STRICT_CPP_TYPE, T)                                                                                                                             \
   };

#define STRICT_CPP_DEFINE_FLOAT_TYPE(STRICT_CPP_TYPE, T)                                                                                                                           \
   struct STRICT_CPP_TYPE : STRICT_CPP_NAMESPACE::detail::strict_cpp_base_t {                                                                                                      \
         using type = T;                                                                                                                                                           \
         T value    = {};                                                                                                                                                          \
                                                                                                                                                                                   \
         DETAIL_STRICT_CPP_MIN_MAX_CONSTANTS(T)                                                                                                                                    \
         DEFINE_STRICT_CPP_FLOAT_CONSTANTS(T)                                                                                                                                      \
         DETAIL_STRICT_CPP_CONSTRUCTORS(STRICT_CPP_TYPE, T)                                                                                                                        \
         DETAIL_STRICT_CPP_OPERATORS(STRICT_CPP_TYPE, T)                                                                                                                           \
         DEFINE_STRICT_CPP_FLOAT_OPERATORS(STRICT_CPP_TYPE, T)                                                                                                                     \
         DETAIL_STRICT_CPP_STRINGS(STRICT_CPP_TYPE, T)                                                                                                                             \
   };

#pragma warning(disable : 4146)

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

#if defined(STRICT_CPP_OPTIONAL_TYPES)
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(size8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(size16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(size32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(size64_t, std::uint64_t);

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

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(capacity_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(capacity8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(capacity16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(capacity32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(capacity64_t, std::uint64_t);

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

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(index_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(index8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(index16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(index32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(index64_t, std::uint64_t);

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

   STRICT_CPP_DEFINE_INTEGRAL_TYPE(dst_offset_t, std::size_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(dst_offset8_t, std::uint8_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(dst_offset16_t, std::uint16_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(dst_offset32_t, std::uint32_t);
   STRICT_CPP_DEFINE_INTEGRAL_TYPE(dst_offset64_t, std::uint64_t);
#endif

   // Float types:
   STRICT_CPP_DEFINE_FLOAT_TYPE(float_t, float);
   STRICT_CPP_DEFINE_FLOAT_TYPE(double_t, double);
   STRICT_CPP_DEFINE_FLOAT_TYPE(long_double_t, long double);
}
