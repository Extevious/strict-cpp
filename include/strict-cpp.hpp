#pragma once

#if !defined(STRICT_CPP_NAMESPACE)
   #define STRICT_CPP_NAMESPACE strict
#endif

#include <cmath>
#include <cstdint>
#include <limits>
#include <string>
#include <type_traits>

namespace STRICT_CPP_NAMESPACE {
   namespace detail {
      // Base type for integral or float type qualifications.
      struct strict_cpp_base_t { };

      // Base type for integral-only type qualifications.
      struct strict_cpp_integral_base_t : strict_cpp_base_t { };

      // Base type for float-only type qualifications.
      struct strict_cpp_float_base_t : strict_cpp_base_t { };

      // Returns true if [Other] is the same as one of the [QualifiedTypes] and if [QualifiedTypes] is a non-zero length.
      template <typename Other, typename... QualifiedTypes>
      concept is_qualified_type = (sizeof...(QualifiedTypes) > 0) && (std::is_same_v<Other, QualifiedTypes> || ...);

      // Returns true if [Other] is the same as one of the [QualifiedTypes] and if [QualifiedTypes] is a non-zero length.
      template <typename Other, typename... QualifiedTypes>
      concept is_qualified_implicit_constructor = is_qualified_type<Other, QualifiedTypes...>;

      // Returns true if [Other] is convertible to [QualifiedType].
      template <typename Other, typename QualifiedType>
      concept is_qualified_explicit_constructor = (std::is_convertible_v<Other, QualifiedType> || std::is_base_of_v<STRICT_CPP_NAMESPACE::detail::strict_cpp_base_t, Other>) &&
                                                  !std::is_same_v<Other, QualifiedType>;

      // Returns true if any of [Types] inherits from the [strict_cpp_base_t] type.
      template <typename... Types>
      concept is_qualified_operator = (std::is_base_of_v<STRICT_CPP_NAMESPACE::detail::strict_cpp_base_t, Types> && ...);

      // Returns true if [Left] inherits from the [strict_cpp_base_t] type and Right is a scalar type.
      template <typename Left, typename Right>
      concept is_qualified_operator_left_only = is_qualified_operator<Left> && std::is_scalar_v<Right>;

      // Returns true if [Left] is a scalar type and Right inherits from the [strict_cpp_base_t] type.
      template <typename Left, typename Right>
      concept is_qualified_operator_right_only = std::is_scalar_v<Left> && is_qualified_operator<Right>;

      // Returns true if any of [Types] inherits from the [strict_cpp_integral_base_t] type.
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
      concept is_qualified_float_operator_left_only = is_qualified_float_operator<Left> &&
                                                      (std::is_arithmetic_v<Right> || std::is_base_of_v<STRICT_CPP_NAMESPACE::detail::strict_cpp_integral_base_t, Right>);

      // Returns true if [Left] is a arithmetic type and [Right] inherits from the [strict_cpp_float_base_t] type.
      template <typename Left, typename Right>
      concept is_qualified_float_operator_right_only = (std::is_arithmetic_v<Left> || std::is_base_of_v<STRICT_CPP_NAMESPACE::detail::strict_cpp_integral_base_t, Left>) &&
                                                       is_qualified_float_operator<Right>;

      // Returns true if [Other] is the same as one of the [QualifiedTypes] and if [QualifiedTypes] is a non-zero length.
      template <typename Other, typename... QualifiedTypes>
      concept is_qualified_assignment_operator = is_qualified_type<Other, QualifiedTypes...>;
   }

#define STRICT_CPP_DEFINE_INTEGRAL_OPERATOR(OPERATOR_TYPE)                                                                                                                         \
   template <typename Left, typename Right>                                                                                                                                        \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_integral_operator<Left, Right>                                                                                           \
   inline constexpr Left operator OPERATOR_TYPE(const Left& left, const Right& right) noexcept {                                                                                   \
      return static_cast<Left>(left.value OPERATOR_TYPE right.value);                                                                                                              \
   }                                                                                                                                                                               \
   template <typename Left, typename Right>                                                                                                                                        \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_integral_operator_left_only<Left, Right>                                                                                 \
   inline constexpr Left operator OPERATOR_TYPE(const Left& left, const Right& right) noexcept {                                                                                   \
      return static_cast<Left>(left.value OPERATOR_TYPE right);                                                                                                                    \
   }                                                                                                                                                                               \
   template <typename Left, typename Right>                                                                                                                                        \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_integral_operator_right_only<Left, Right>                                                                                \
   inline constexpr Left operator OPERATOR_TYPE(const Left& left, const Right& right) noexcept {                                                                                   \
      return static_cast<Left>(left OPERATOR_TYPE right.value);                                                                                                                    \
   }

#define STRICT_CPP_DEFINE_FLOAT_OPERATOR(OPERATOR_TYPE)                                                                                                                            \
   template <typename Left, typename Right>                                                                                                                                        \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator<Left, Right>                                                                                              \
   inline constexpr Left operator OPERATOR_TYPE(const Left& left, const Right& right) noexcept {                                                                                   \
      return static_cast<Left>(left.value OPERATOR_TYPE right.value);                                                                                                              \
   }                                                                                                                                                                               \
   template <typename Left, typename Right>                                                                                                                                        \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator_left_only<Left, Right>                                                                                    \
   inline constexpr Left operator OPERATOR_TYPE(const Left& left, const Right& right) noexcept {                                                                                   \
      return static_cast<Left>(left.value OPERATOR_TYPE right);                                                                                                                    \
   }                                                                                                                                                                               \
   template <typename Left, typename Right>                                                                                                                                        \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator_right_only<Left, Right>                                                                                   \
   inline constexpr Left operator OPERATOR_TYPE(const Left& left, const Right& right) noexcept {                                                                                   \
      return static_cast<Left>(left OPERATOR_TYPE right.value);                                                                                                                    \
   }

#define STRICT_CPP_DEFINE_COMPOUND_INTEGRAL_OPERATOR(OPERATOR_TYPE)                                                                                                                \
   template <typename Left, typename Right>                                                                                                                                        \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_integral_operator<Left, Right>                                                                                           \
   inline constexpr Left& operator OPERATOR_TYPE(Left & left, const Right & right) noexcept {                                                                                      \
      left.value OPERATOR_TYPE right.value;                                                                                                                                        \
      return left;                                                                                                                                                                 \
   }                                                                                                                                                                               \
   template <typename Left, typename Right>                                                                                                                                        \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_integral_operator_left_only<Left, Right>                                                                                 \
   inline constexpr Left& operator OPERATOR_TYPE(Left & left, const Right & right) noexcept {                                                                                      \
      left.value OPERATOR_TYPE right;                                                                                                                                              \
      return left;                                                                                                                                                                 \
   }                                                                                                                                                                               \
   template <typename Left, typename Right>                                                                                                                                        \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_integral_operator_right_only<Left, Right>                                                                                \
   inline constexpr Left& operator OPERATOR_TYPE(Left & left, const Right & right) noexcept {                                                                                      \
      left OPERATOR_TYPE right.value;                                                                                                                                              \
      return left;                                                                                                                                                                 \
   }

#define STRICT_CPP_DEFINE_COMPOUND_FLOAT_OPERATOR(OPERATOR_TYPE)                                                                                                                   \
   template <typename Left, typename Right>                                                                                                                                        \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator<Left, Right>                                                                                              \
   inline constexpr Left& operator OPERATOR_TYPE(Left & left, const Right & right) noexcept {                                                                                      \
      left.value OPERATOR_TYPE right.value;                                                                                                                                        \
      return left;                                                                                                                                                                 \
   }                                                                                                                                                                               \
   template <typename Left, typename Right>                                                                                                                                        \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator_left_only<Left, Right>                                                                                    \
   inline constexpr Left& operator OPERATOR_TYPE(Left & left, const Right & right) noexcept {                                                                                      \
      left.value OPERATOR_TYPE right;                                                                                                                                              \
      return left;                                                                                                                                                                 \
   }                                                                                                                                                                               \
   template <typename Left, typename Right>                                                                                                                                        \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator_right_only<Left, Right>                                                                                   \
   inline constexpr Left& operator OPERATOR_TYPE(Left & left, const Right & right) noexcept {                                                                                      \
      left OPERATOR_TYPE right.value;                                                                                                                                              \
      return left;                                                                                                                                                                 \
   }

#define STRICT_CPP_DEFINE_COMPARISON_OPERATOR(OPERATOR_TYPE)                                                                                                                       \
   template <typename Left, typename Right>                                                                                                                                        \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator<Left, Right>                                                                                                    \
   inline constexpr bool operator OPERATOR_TYPE(const Left& left, const Right& right) noexcept {                                                                                   \
      return left.value OPERATOR_TYPE right.value;                                                                                                                                 \
   }                                                                                                                                                                               \
   template <typename Left, typename Right>                                                                                                                                        \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator_left_only<Left, Right>                                                                                          \
   inline constexpr bool operator OPERATOR_TYPE(const Left& left, const Right& right) noexcept {                                                                                   \
      return left.value OPERATOR_TYPE right;                                                                                                                                       \
   }                                                                                                                                                                               \
   template <typename Left, typename Right>                                                                                                                                        \
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator_right_only<Left, Right>                                                                                         \
   inline constexpr bool operator OPERATOR_TYPE(const Left& left, const Right& right) noexcept {                                                                                   \
      return left OPERATOR_TYPE right.value;                                                                                                                                       \
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
   inline constexpr Type operator+(const Type& value) noexcept {
      return static_cast<Type>(+value.value);
   }

   template <typename Type>
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator<Type>
   inline constexpr Type operator-(const Type& value) noexcept {
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
   inline constexpr Type operator~(const Type& value) noexcept {
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
   inline constexpr Left operator%(const Left& left, const Right& right) noexcept {
      if constexpr (__cplusplus >= 202207L) return static_cast<Left>(std::fmod(left.value, right.value));
      else return static_cast<Left>(std::fmodl(left.value, right.value));
   }

   template <typename Left, typename Right>
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator_left_only<Left, Right>
   inline constexpr Left operator%(const Left& left, const Right& right) noexcept {
      if constexpr (__cplusplus >= 202207L) return static_cast<Left>(std::fmod(left.value, right));
      else return static_cast<Left>(std::fmodl(left.value, right));
   }

   template <typename Left, typename Right>
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator_right_only<Left, Right>
   inline constexpr Left operator%(const Left& left, const Right& right) noexcept {
      if constexpr (__cplusplus >= 202207L) return static_cast<Left>(std::fmod(left, right.value));
      else return static_cast<Left>(std::fmodl(left, right.value));
   }

   template <typename Left, typename Right>
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator<Left, Right>
   inline constexpr Left& operator%=(Left& left, const Right& right) noexcept {
      if constexpr (__cplusplus >= 202207L) left.value = static_cast<typename Left::type>(std::fmod(left.value, right.value));
      else left.value = static_cast<typename Left::type>(std::fmodl(left.value, right.value));

      return left;
   }

   template <typename Left, typename Right>
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator_left_only<Left, Right>
   inline constexpr Left& operator%=(Left& left, const Right& right) noexcept {
      if constexpr (__cplusplus >= 202207L) left.value = static_cast<typename Left::type>(std::fmod(left.value, right));
      else left.value = static_cast<typename Left::type>(std::fmodl(left.value, right));

      return left;
   }

   template <typename Left, typename Right>
      requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_operator_right_only<Left, Right>
   inline constexpr Left& operator%=(Left& left, const Right& right) noexcept {
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
      requires std::is_integral_v<Type>
   struct strict_integral_type : STRICT_CPP_NAMESPACE::detail::strict_cpp_integral_base_t {
         inline constexpr static Type min = std::numeric_limits<Type>::min();
         inline constexpr static Type max = std::numeric_limits<Type>::max();

         using type = Type;
         Type value = {};

         /// @brief Default constructor.
         inline constexpr strict_integral_type() noexcept = default;

         /// @brief Implicit constructor.
         /// @tparam Other The implicitly-convertible type.
         /// @param other The implicitly-convertible value.
         template <typename Other>
            requires STRICT_CPP_NAMESPACE::detail::is_qualified_implicit_constructor<Other, Type, QualifiedTypes...>
         inline constexpr strict_integral_type(const Other& other) noexcept :
            value(static_cast<Type>(other)) { }

         /// @brief Explicit constructor.
         /// @tparam Other The explicitly-convertible type.
         /// @param other The explicitly-convertible value.
         template <typename Other>
            requires STRICT_CPP_NAMESPACE::detail::is_qualified_explicit_constructor<Other, Type>
         inline constexpr explicit strict_integral_type(const Other& other) noexcept :
            value(static_cast<Type>(other)) { }

         /// @brief Assignment operator.
         /// @tparam Other The assignment type.
         /// @returns auto&
         template <typename Other>
            requires STRICT_CPP_NAMESPACE::detail::is_qualified_assignment_operator<Other, Type, QualifiedTypes...>
         inline constexpr auto& operator=(const Other& other) noexcept {
            this->value = other.value;
            return *this;
         }

         /// @brief Implicit conversion operator converts to the same encapsulated type only.
         /// @returns Type
         inline constexpr operator Type() const noexcept { return this->value; }

         /// @brief Explicit conversion operator.
         /// @tparam Other The type to convert to.
         /// @returns Other
         template <typename Other>
            requires std::is_convertible_v<Type, Other>
         inline constexpr explicit operator Other() const noexcept {
            return static_cast<Other>(this->value);
         }

         /// @brief Conversion function.
         /// @tparam Other The type to convert to.
         /// @returns Other
         template <typename Other>
            requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator<Other>
         inline constexpr Other as() const noexcept {
            return static_cast<Other>(this->value);
         }

         /// @brief Converts to a human-readable string representing the current value.
         /// @returns std::string
         inline std::string to_string() const { return std::to_string(value); }

         /// @brief Converts to a human-readable wide string representing the current value.
         /// @returns std::wstring
         inline std::wstring to_wstring() const { return std::to_wstring(value); }
   };

   template <typename Type, typename... QualifiedTypes>
      requires std::is_floating_point_v<Type>
   struct strict_float_type : STRICT_CPP_NAMESPACE::detail::strict_cpp_float_base_t {
         inline constexpr static Type min         = std::numeric_limits<Type>::min();
         inline constexpr static Type max         = std::numeric_limits<Type>::max();
         inline constexpr static Type lowest      = std::numeric_limits<Type>::lowest();
         inline constexpr static Type qNaN        = std::numeric_limits<Type>::quiet_NaN();
         inline constexpr static Type sNaN        = std::numeric_limits<Type>::signaling_NaN();
         inline constexpr static Type posInfinity = std::numeric_limits<Type>::infinity();
         inline constexpr static Type negInfinity = -std::numeric_limits<Type>::infinity();
         inline constexpr static Type epsilon     = std::numeric_limits<Type>::epsilon();
         inline constexpr static Type roundError  = std::numeric_limits<Type>::round_error();
         inline constexpr static Type denormal    = std::numeric_limits<Type>::denorm_min();

         using type = Type;
         Type value = {};

         /// @brief Default constructor.
         inline constexpr strict_float_type() noexcept = default;

         /// @brief Implicit constructor.
         /// @tparam Other The implicitly-convertible type.
         /// @param other The implicitly-convertible value.
         template <typename Other>
            requires STRICT_CPP_NAMESPACE::detail::is_qualified_implicit_constructor<Other, Type, QualifiedTypes...>
         inline constexpr strict_float_type(const Other& other) noexcept :
            value(static_cast<Type>(other)) { }

         /// @brief Explicit constructor.
         /// @tparam Other The explicitly-convertible type.
         /// @param other The explicitly-convertible value.
         template <typename Other>
            requires STRICT_CPP_NAMESPACE::detail::is_qualified_explicit_constructor<Other, Type>
         inline constexpr explicit strict_float_type(const Other& other) noexcept :
            value(static_cast<Type>(other)) { }

         /// @brief Assignment operator.
         /// @tparam Other The assignment type.
         /// @returns auto&
         template <typename Other>
            requires STRICT_CPP_NAMESPACE::detail::is_qualified_assignment_operator<Other, Type, QualifiedTypes...>
         inline constexpr auto& operator=(const Other& other) noexcept {
            this->value = other.value;
            return *this;
         }

         /// @brief Implicit conversion operator converts to the same encapsulated type only.
         /// @returns Type
         inline constexpr operator Type() const noexcept { return this->value; }

         /// @brief Explicit conversion operator.
         /// @tparam Other The type to convert to.
         /// @returns Other
         template <typename Other>
            requires std::is_convertible_v<Type, Other>
         inline constexpr explicit operator Other() const noexcept {
            return static_cast<Other>(this->value);
         }

         /// @brief Conversion function.
         /// @tparam Other The type to convert to.
         /// @returns Other
         template <typename Other>
            requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator<Other>
         inline constexpr Other as() const noexcept {
            return static_cast<Other>(this->value);
         }

         /// @brief Converts to a human-readable string representing the current value.
         /// @returns std::string
         inline std::string to_string() const { return std::to_string(value); }

         /// @brief Converts to a human-readable wide string representing the current value.
         /// @returns std::wstring
         inline std::wstring to_wstring() const { return std::to_wstring(value); }
   };
}

#define STRICT_CPP_DEFINE_INTEGRAL_TYPE(NAME, TYPE, QUALIFIED_TYPES...)                                                                                                            \
   struct NAME : STRICT_CPP_NAMESPACE::strict_integral_type<TYPE, QUALIFIED_TYPES> {                                                                                               \
         using STRICT_CPP_NAMESPACE::strict_integral_type<TYPE, QUALIFIED_TYPES>::strict_integral_type;                                                                            \
   };

#define STRICT_CPP_DEFINE_FLOAT_TYPE(NAME, TYPE, QUALIFIED_TYPES...)                                                                                                               \
   struct NAME : STRICT_CPP_NAMESPACE::strict_float_type<TYPE, QUALIFIED_TYPES> {                                                                                                  \
         using STRICT_CPP_NAMESPACE::strict_float_type<TYPE, QUALIFIED_TYPES>::strict_float_type;                                                                                  \
   };

#pragma warning(disable : 4146)

// =============================================================================
// Pre-defined types
// =============================================================================

// Common integral types:
namespace STRICT_CPP_NAMESPACE {
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

   // Optional integral types:
#if defined(STRICT_CPP_OPTIONAL_TYPES)
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
