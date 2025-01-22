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

      /// @brief Integral value-encapsulating struct for type qualifications (ex; std::formatter<>).
      /// @tparam Type The encapsulated type.
      template <typename Type>
         requires std::is_integral_v<Type>
      struct strict_integral_value_t : STRICT_CPP_NAMESPACE::detail::strict_cpp_integral_base_t {
            inline constexpr static Type min = std::numeric_limits<Type>::min();
            inline constexpr static Type max = std::numeric_limits<Type>::max();

            using type = Type;
            Type value = {};

            inline constexpr strict_integral_value_t() noexcept = default;

            inline constexpr strict_integral_value_t(const Type& value) noexcept :
               value(value) { }

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

            /// @brief Converts to a human-readable string representing the current value.
            /// @returns std::string
            inline std::string to_string() const { return std::to_string(this->value); }

            /// @brief Converts to a human-readable wide string representing the current value.
            /// @returns std::wstring
            inline std::wstring to_wstring() const { return std::to_wstring(this->value); }
      };

      /// @brief Floating-point value-encapsulating struct for type qualifications (ex; std::formatter<>).
      /// @tparam Type The encapsulated type.
      template <typename Type>
         requires std::is_floating_point_v<Type>
      struct strict_float_value_t : STRICT_CPP_NAMESPACE::detail::strict_cpp_float_base_t {
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

            inline constexpr strict_float_value_t() noexcept = default;

            inline constexpr strict_float_value_t(const Type& value) noexcept :
               value(value) { }

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

            /// @brief Converts to a human-readable string representing the current value.
            /// @returns std::string
            inline std::string to_string() const { return std::to_string(this->value); }

            /// @brief Converts to a human-readable wide string representing the current value.
            /// @returns std::wstring
            inline std::wstring to_wstring() const { return std::to_wstring(this->value); }
      };

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

      // Returns true if [Other] is integral or integral strict type.
      template <typename Other>
      concept is_qualified_integral_assignment_operator = std::is_integral_v<Other> || std::is_base_of_v<Other, STRICT_CPP_NAMESPACE::detail::strict_cpp_integral_base_t>;

      // Returns true if [Other] is a float or float strict type.
      template <typename Other>
      concept is_qualified_float_assignment_operator = std::is_floating_point_v<Other> || std::is_base_of_v<Other, STRICT_CPP_NAMESPACE::detail::strict_cpp_float_base_t>;
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
      requires std::is_integral_v<Type> && std::is_trivial_v<Type>
   struct strict_integral_type : STRICT_CPP_NAMESPACE::detail::strict_integral_value_t<Type> {
         /// @brief Default constructor.
         inline constexpr strict_integral_type() noexcept = default;

         /// @brief Implicit copy constructor.
         /// @tparam Other The implicitly-convertible type.
         /// @param other The implicitly-convertible value.
         template <typename Other>
            requires STRICT_CPP_NAMESPACE::detail::is_qualified_implicit_constructor<Other, Type, QualifiedTypes...>
         inline constexpr strict_integral_type(const Other& other) noexcept :
            STRICT_CPP_NAMESPACE::detail::strict_integral_value_t<Type>(static_cast<Type>(other)) { }

         /// @brief Explicit copy constructor.
         /// @tparam Other The explicitly-convertible type.
         /// @param other The explicitly-convertible value.
         template <typename Other>
            requires STRICT_CPP_NAMESPACE::detail::is_qualified_explicit_constructor<Other, Type>
         inline constexpr explicit strict_integral_type(const Other& other) noexcept :
            STRICT_CPP_NAMESPACE::detail::strict_integral_value_t<Type>(static_cast<Type>(other)) { }

         /// @brief Assignment operator.
         /// @tparam Other The assignment type.
         /// @returns strict_integral_type&
         template <typename Other>
            requires STRICT_CPP_NAMESPACE::detail::is_qualified_integral_assignment_operator<Other>
         inline constexpr strict_integral_type& operator=(const Other& other) noexcept {
            if constexpr (std::is_integral_v<Other>) this->value = other;
            else this->value = other.value;
            return *this;
         }

         /// @brief Conversion function.
         /// @tparam Other The type to convert to.
         /// @returns Other
         template <typename Other>
            requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator<Other>
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
         inline constexpr strict_float_type(const Other& other) noexcept :
            STRICT_CPP_NAMESPACE::detail::strict_float_value_t<Type>(static_cast<Type>(other)) { }

         /// @brief Explicit copy constructor.
         /// @tparam Other The explicitly-convertible type.
         /// @param other The explicitly-convertible value.
         template <typename Other>
            requires STRICT_CPP_NAMESPACE::detail::is_qualified_explicit_constructor<Other, Type>
         inline constexpr explicit strict_float_type(const Other& other) noexcept :
            STRICT_CPP_NAMESPACE::detail::strict_float_value_t<Type>(static_cast<Type>(other)) { }

         /// @brief Assignment operator.
         /// @tparam Other The assignment type.
         /// @returns strict_float_type&
         template <typename Other>
            requires STRICT_CPP_NAMESPACE::detail::is_qualified_float_assignment_operator<Other>
         inline constexpr strict_float_type& operator=(const Other& other) noexcept {
            if constexpr (std::is_floating_point_v<Other>) this->value = other;
            else this->value = other.value;
            return *this;
         }

         /// @brief Conversion function.
         /// @tparam Other The type to convert to.
         /// @returns Other
         template <typename Other>
            requires STRICT_CPP_NAMESPACE::detail::is_qualified_operator<Other>
         inline constexpr Other as() const noexcept {
            return static_cast<Other>(this->value);
         }
   };

   template <typename Type, typename... QualifiedTypes>
      requires(sizeof(Type) != 0) && (!std::is_reference_v<Type>)
   struct strict_alias_type : STRICT_CPP_NAMESPACE::detail::strict_cpp_alias_base_t {
         using type = Type;
         Type value = {};

         /// @brief Default constructor.
         inline constexpr strict_alias_type() noexcept = default;

         /// @brief Move constructor.
         /// @tparam Other The moveable type.
         /// @param other The value to move.
         template <typename Other>
            requires std::is_convertible_v<Other, Type>
         inline constexpr strict_alias_type(Other&& other) noexcept :
            value(std::move(other)) { }

         /// @brief Copy constructor.
         /// @tparam Other The copyable type.
         /// @param other The value to copy.
         template <typename Other>
            requires std::is_convertible_v<Other, Type>
         inline constexpr strict_alias_type(const Other& other) noexcept :
            value(static_cast<Type>(other)) { }

         /// @brief Move-assignment operator.
         /// @tparam Other The assignment type.
         /// @param other The move-assignment value.
         /// @returns auto&
         template <typename Other>
            requires std::is_convertible_v<Other, Type>
         inline constexpr auto& operator=(Other&& other) noexcept {
            this->value = std::move(other.value);
            return *this;
         }

         /// @brief Copy-assignment operator.
         /// @tparam Other The assignment type.
         /// @param other The copy-assignment value.
         /// @returns auto&
         template <typename Other>
            requires std::is_convertible_v<Other, Type>
         inline constexpr auto& operator=(const Other& other) noexcept {
            this->value = other.value;
            return *this;
         }

         /// @brief Implicit ref conversion operator converts to the same encapsulated type only.
         /// @returns Type&
         inline constexpr operator Type&() noexcept { return this->value; }

         /// @brief Implicit ref conversion operator converts to the same encapsulated type only.
         /// @returns Type&
         inline constexpr operator Type&() const noexcept { return this->value; }

         /// @brief Indirect member access operator.
         /// @returns Type&
         inline constexpr Type* operator->() noexcept { return &this->value; }

         /// @brief Indirect member access operator.
         /// @returns Type&
         inline constexpr const Type* operator->() const noexcept { return &this->value; }

         /// @brief Subscript operator.
         /// @returns Type&
         template <typename _ = void>
            requires(!std::is_void_v<decltype(std::declval<Type>().operator[](0))>)
         inline constexpr auto& operator[](const auto& index) noexcept {
            return this->value[index];
         }

         /// @brief Subscript operator.
         /// @returns Type&
         template <typename _ = void>
            requires(!std::is_void_v<decltype(std::declval<Type>().operator[](0))>)
         inline constexpr const auto& operator[](const auto& index) const noexcept {
            return this->value[index];
         }

         /// @brief Explicit copy conversion operator.
         /// @tparam Other The type to convert to.
         /// @returns Other
         template <typename Other>
            requires std::is_convertible_v<Type, Other>
         inline constexpr explicit operator Other() const noexcept {
            return static_cast<Other>(this->value);
         }
   };
}

#define STRICT_CPP_DEFINE_FORMATTER(TYPE)                                                                                                                                          \
   template <>                                                                                                                                                                     \
   struct _STD formatter<STRICT_CPP_NAMESPACE::TYPE> {                                                                                                                             \
         inline constexpr auto parse(const _STD format_parse_context& context) const noexcept { return context.begin(); }                                                          \
         inline constexpr auto parse(const _STD wformat_parse_context& context) const noexcept { return context.begin(); }                                                         \
         inline auto           format(const STRICT_CPP_NAMESPACE::TYPE& value, _STD format_context& context) const { return _STD format_to(context.out(), "{}", value.value); }    \
         inline auto           format(const STRICT_CPP_NAMESPACE::TYPE& value, _STD wformat_context& context) const { return _STD format_to(context.out(), L"{}", value.value); }  \
   };

#define STRICT_CPP_DEFINE_INTEGRAL_TYPE(NAME, TYPE, QUALIFIED_TYPES...)                                                                                                            \
   namespace STRICT_CPP_NAMESPACE {                                                                                                                                                \
      struct NAME : STRICT_CPP_NAMESPACE::strict_integral_type<TYPE, QUALIFIED_TYPES> {                                                                                            \
            using STRICT_CPP_NAMESPACE::strict_integral_type<TYPE, QUALIFIED_TYPES>::strict_integral_type;                                                                         \
            using STRICT_CPP_NAMESPACE::strict_integral_type<TYPE, QUALIFIED_TYPES>::operator=;                                                                                    \
      };                                                                                                                                                                           \
   }                                                                                                                                                                               \
   STRICT_CPP_DEFINE_FORMATTER(NAME)

#define STRICT_CPP_DEFINE_FLOAT_TYPE(NAME, TYPE, QUALIFIED_TYPES...)                                                                                                               \
   namespace STRICT_CPP_NAMESPACE {                                                                                                                                                \
      struct NAME : STRICT_CPP_NAMESPACE::strict_float_type<TYPE, QUALIFIED_TYPES> {                                                                                               \
            using STRICT_CPP_NAMESPACE::strict_float_type<TYPE, QUALIFIED_TYPES>::strict_float_type;                                                                               \
            using STRICT_CPP_NAMESPACE::strict_float_type<TYPE, QUALIFIED_TYPES>::operator=;                                                                                       \
      };                                                                                                                                                                           \
   }                                                                                                                                                                               \
   STRICT_CPP_DEFINE_FORMATTER(NAME)

#define STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(NAME, QUALIFIED_TYPES...)                                                                                                          \
   namespace STRICT_CPP_NAMESPACE {                                                                                                                                                \
      template <typename T>                                                                                                                                                        \
         requires std::is_integral_v<T>                                                                                                                                            \
      struct NAME : STRICT_CPP_NAMESPACE::strict_integral_type<T, QUALIFIED_TYPES> {                                                                                               \
            using STRICT_CPP_NAMESPACE::strict_integral_type<T, QUALIFIED_TYPES>::strict_integral_type;                                                                            \
            using STRICT_CPP_NAMESPACE::strict_integral_type<T, QUALIFIED_TYPES>::operator=;                                                                                       \
      };                                                                                                                                                                           \
   }                                                                                                                                                                               \
   STRICT_CPP_DEFINE_FORMATTER(NAME<char>)                                                                                                                                         \
   STRICT_CPP_DEFINE_FORMATTER(NAME<unsigned char>)                                                                                                                                \
   STRICT_CPP_DEFINE_FORMATTER(NAME<unsigned short>)                                                                                                                               \
   STRICT_CPP_DEFINE_FORMATTER(NAME<unsigned int>)                                                                                                                                 \
   STRICT_CPP_DEFINE_FORMATTER(NAME<unsigned long>)                                                                                                                                \
   STRICT_CPP_DEFINE_FORMATTER(NAME<unsigned long long>)                                                                                                                           \
   STRICT_CPP_DEFINE_FORMATTER(NAME<signed char>)                                                                                                                                  \
   STRICT_CPP_DEFINE_FORMATTER(NAME<signed short>)                                                                                                                                 \
   STRICT_CPP_DEFINE_FORMATTER(NAME<signed int>)                                                                                                                                   \
   STRICT_CPP_DEFINE_FORMATTER(NAME<signed long>)                                                                                                                                  \
   STRICT_CPP_DEFINE_FORMATTER(NAME<signed long long>)

#define STRICT_CPP_DEFINE_DYNAMIC_FLOAT_TYPE(NAME, QUALIFIED_TYPES...)                                                                                                             \
   namespace STRICT_CPP_NAMESPACE {                                                                                                                                                \
      template <typename T>                                                                                                                                                        \
         requires std::is_floating_point_v<T>                                                                                                                                      \
      struct NAME : STRICT_CPP_NAMESPACE::strict_float_type<T, QUALIFIED_TYPES> {                                                                                                  \
            using STRICT_CPP_NAMESPACE::strict_float_type<T, QUALIFIED_TYPES>::strict_float_type;                                                                                  \
            using STRICT_CPP_NAMESPACE::strict_float_type<T, QUALIFIED_TYPES>::operator=;                                                                                          \
      };                                                                                                                                                                           \
   }                                                                                                                                                                               \
   STRICT_CPP_DEFINE_FORMATTER(NAME<float>)                                                                                                                                        \
   STRICT_CPP_DEFINE_FORMATTER(NAME<double>)                                                                                                                                       \
   STRICT_CPP_DEFINE_FORMATTER(NAME<long double>)

#define STRICT_CPP_DEFINE_ALIAS_TYPE(NAME, TYPE)                                                                                                                                   \
   namespace STRICT_CPP_NAMESPACE {                                                                                                                                                \
      struct NAME : STRICT_CPP_NAMESPACE::strict_alias_type<TYPE> {                                                                                                                \
            using STRICT_CPP_NAMESPACE::strict_alias_type<TYPE>::strict_alias_type;                                                                                                \
            using STRICT_CPP_NAMESPACE::strict_alias_type<TYPE>::operator=;                                                                                                        \
      };                                                                                                                                                                           \
   }

// =============================================================================
// Pre-defined types
// =============================================================================

// Common integral types:
STRICT_CPP_DEFINE_INTEGRAL_TYPE(char_t, char);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(schar_t, std::int8_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uchar_t, std::uint8_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(short_t, std::int16_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(ushort_t, std::uint16_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(long_t, std::int64_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(ulong_t, std::uint64_t);

STRICT_CPP_DEFINE_INTEGRAL_TYPE(size_t, std::size_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(size8_t, std::uint8_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(size16_t, std::uint16_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(size32_t, std::uint32_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(size64_t, std::uint64_t);

STRICT_CPP_DEFINE_INTEGRAL_TYPE(int_t, std::int64_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int8_t, std::int8_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int16_t, std::int16_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int32_t, std::int32_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int64_t, std::int64_t);

STRICT_CPP_DEFINE_INTEGRAL_TYPE(int_least_t, std::int_least64_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int_least8_t, std::int_least8_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int_least16_t, std::int_least16_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int_least32_t, std::int_least32_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int_least64_t, std::int_least64_t);

STRICT_CPP_DEFINE_INTEGRAL_TYPE(int_fast_t, std::int_fast64_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int_fast8_t, std::int_fast8_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int_fast16_t, std::int_fast16_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int_fast32_t, std::int_fast32_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(int_fast64_t, std::int_fast64_t);

STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint_t, std::uint64_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint8_t, std::uint8_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint16_t, std::uint16_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint32_t, std::uint32_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint64_t, std::uint64_t);

STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint_least_t, std::uint_least64_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint_least8_t, std::uint_least8_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint_least16_t, std::uint_least16_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint_least32_t, std::uint_least32_t);
STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint_least64_t, std::uint_least64_t);

STRICT_CPP_DEFINE_INTEGRAL_TYPE(uint_fast_t, std::uint_fast64_t);
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

// Dynamic "any" integral types:
STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
   any_int_t,
   char,
   std::int8_t,
   std::int16_t,
   std::int32_t,
   std::int64_t,
   std::uint8_t,
   std::uint16_t,
   std::uint32_t,
   std::uint64_t,
   STRICT_CPP_NAMESPACE::char_t,
   STRICT_CPP_NAMESPACE::schar_t,
   STRICT_CPP_NAMESPACE::uchar_t,
   STRICT_CPP_NAMESPACE::short_t,
   STRICT_CPP_NAMESPACE::ushort_t,
   STRICT_CPP_NAMESPACE::int_t,
   STRICT_CPP_NAMESPACE::uint_t,
   STRICT_CPP_NAMESPACE::long_t,
   STRICT_CPP_NAMESPACE::ulong_t,
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
   any_int8_t,
   char,
   std::int8_t,
   std::uint8_t,
   STRICT_CPP_NAMESPACE::char_t,
   STRICT_CPP_NAMESPACE::schar_t,
   STRICT_CPP_NAMESPACE::uchar_t,
   STRICT_CPP_NAMESPACE::int8_t,
   STRICT_CPP_NAMESPACE::uint8_t
);

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
   any_int32_t,
   std::int32_t,
   std::uint32_t,
   STRICT_CPP_NAMESPACE::int_t,
   STRICT_CPP_NAMESPACE::uint_t,
   STRICT_CPP_NAMESPACE::int32_t,
   STRICT_CPP_NAMESPACE::uint32_t
);
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
   any_size_t,
   std::size_t,
   STRICT_CPP_NAMESPACE::size_t,
   STRICT_CPP_NAMESPACE::size8_t,
   STRICT_CPP_NAMESPACE::size16_t,
   STRICT_CPP_NAMESPACE::size32_t,
   STRICT_CPP_NAMESPACE::size64_t
);

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
   any_signed_int_t,
   std::int8_t,
   std::int16_t,
   std::int32_t,
   std::int64_t,
   STRICT_CPP_NAMESPACE::int_t,
   STRICT_CPP_NAMESPACE::int8_t,
   STRICT_CPP_NAMESPACE::int16_t,
   STRICT_CPP_NAMESPACE::int32_t,
   STRICT_CPP_NAMESPACE::int64_t
);

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(any_signed_int8_t, std::int8_t, STRICT_CPP_NAMESPACE::schar_t, STRICT_CPP_NAMESPACE::int8_t);

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(any_signed_int16_t, std::int16_t, STRICT_CPP_NAMESPACE::short_t, STRICT_CPP_NAMESPACE::int16_t);

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(any_signed_int32_t, std::int32_t, STRICT_CPP_NAMESPACE::int_t, STRICT_CPP_NAMESPACE::int32_t);

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(any_signed_int64_t, std::int64_t, STRICT_CPP_NAMESPACE::long_t, STRICT_CPP_NAMESPACE::int64_t);

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(
   any_unsigned_int_t,
   std::uint8_t,
   std::uint16_t,
   std::uint32_t,
   std::uint64_t,
   STRICT_CPP_NAMESPACE::uint_t,
   STRICT_CPP_NAMESPACE::uint8_t,
   STRICT_CPP_NAMESPACE::uint16_t,
   STRICT_CPP_NAMESPACE::uint32_t,
   STRICT_CPP_NAMESPACE::uint64_t
);

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(any_unsigned_int8_t, std::uint8_t, STRICT_CPP_NAMESPACE::uchar_t, STRICT_CPP_NAMESPACE::uint8_t);

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(any_unsigned_int16_t, std::uint16_t, STRICT_CPP_NAMESPACE::ushort_t, STRICT_CPP_NAMESPACE::uint16_t);

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(any_unsigned_int32_t, std::uint32_t, STRICT_CPP_NAMESPACE::uint_t, STRICT_CPP_NAMESPACE::uint32_t);

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(any_unsigned_int64_t, std::uint64_t, STRICT_CPP_NAMESPACE::ulong_t, STRICT_CPP_NAMESPACE::uint64_t);

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

STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(any_intmax_t, std::uintmax_t, std::intmax_t, STRICT_CPP_NAMESPACE::intmax_t, STRICT_CPP_NAMESPACE::uintmax_t);
STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(any_intptr_t, std::uintptr_t, std::intptr_t, STRICT_CPP_NAMESPACE::intptr_t, STRICT_CPP_NAMESPACE::uintptr_t);
STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(any_byte_t, char, std::int8_t, std::uint8_t, STRICT_CPP_NAMESPACE::byte_t, STRICT_CPP_NAMESPACE::sbyte_t);

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
   #define STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(NAME)                                                                                                                          \
      STRICT_CPP_DEFINE_INTEGRAL_TYPE(NAME##_t, std::size_t);                                                                                                                      \
      STRICT_CPP_DEFINE_INTEGRAL_TYPE(NAME##8_t, std::uint8_t);                                                                                                                    \
      STRICT_CPP_DEFINE_INTEGRAL_TYPE(NAME##16_t, std::uint16_t);                                                                                                                  \
      STRICT_CPP_DEFINE_INTEGRAL_TYPE(NAME##32_t, std::uint32_t);                                                                                                                  \
      STRICT_CPP_DEFINE_INTEGRAL_TYPE(NAME##64_t, std::uint64_t);                                                                                                                  \
      STRICT_CPP_DEFINE_DYNAMIC_INTEGRAL_TYPE(                                                                                                                                     \
         any_##NAME##_t,                                                                                                                                                           \
         std::uint8_t,                                                                                                                                                             \
         std::uint16_t,                                                                                                                                                            \
         std::uint32_t,                                                                                                                                                            \
         std::uint64_t,                                                                                                                                                            \
         STRICT_CPP_NAMESPACE::NAME##_t,                                                                                                                                           \
         STRICT_CPP_NAMESPACE::NAME##8_t,                                                                                                                                          \
         STRICT_CPP_NAMESPACE::NAME##16_t,                                                                                                                                         \
         STRICT_CPP_NAMESPACE::NAME##32_t,                                                                                                                                         \
         STRICT_CPP_NAMESPACE::NAME##64_t                                                                                                                                          \
      );

// size_t
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(offset_size);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(begin_size);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(begin_offset_size);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(end_size);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(end_offset_size);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(src_size);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(src_offset_size);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(dst_size);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(dst_offset_size);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(lhs_size);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(lhs_offset_size);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(rhs_size);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(rhs_offset_size);

// count_t
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(count);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(offset_count);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(src_count);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(src_offset_count);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(begin_count);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(begin_offset_count);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(lhs_count);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(lhs_offset_count);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(dst_count);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(dst_offset_count);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(end_count);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(end_offset_count);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(rhs_count);
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
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(index_offset);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(begin_index);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(begin_index_offset);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(end_index);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(end_index_offset);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(src_index);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(src_index_offset);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(dst_index);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(dst_index_offset);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(lhs_index);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(lhs_index_offset);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(rhs_index);
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(rhs_index_offset);

// capacity_t
STRICT_CPP_DEFINE_OPTIONAL_TYPE_UNSIGNED(capacity);
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-pragmas"

#pragma pop_macro("min") // NOLINT
#pragma pop_macro("max") // NOLINT

#pragma GCC diagnostic pop
