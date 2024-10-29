#include "strict-cpp.hpp"

#include <iostream>
#include <string>
#include <vector>

STRICT_CPP_DEFINE_ALIAS_TYPE(some_string_A, std::string);
STRICT_CPP_DEFINE_ALIAS_TYPE(some_string_B, std::string);
STRICT_CPP_DEFINE_ALIAS_TYPE(some_ints_A, std::vector<int>);
STRICT_CPP_DEFINE_ALIAS_TYPE(some_ints_B, std::vector<int>);

// some_string_A
void example0(const STRICT_CPP_NAMESPACE::some_string_A& string) { std::cout << "\nexample0:\n" << string.value << "\n"; }

// some_string_B
void example0(const STRICT_CPP_NAMESPACE::some_string_B& string) { std::cout << "\nexample0:\n" << string.value << "\n"; }

// some_ints_A
void example1(const STRICT_CPP_NAMESPACE::some_ints_A& ints) {
   std::cout << "\nexample1:\n";

   for (const int i : ints.value) {
      std::cout << i << "\n";
   }
}

// some_ints_B
void example1(const STRICT_CPP_NAMESPACE::some_ints_B& ints) {
   std::cout << "\nexample1:\n";

   for (const int i : ints.value) {
      std::cout << i << "\n";
   }
}

// int vectors
void example2(const std::vector<int>& ints0, const std::vector<int>& ints1) {
   std::cout << "\nexample2:\n";

   for (STRICT_CPP_NAMESPACE::size_t i = 0ULL; i < 5; i++) {
      std::cout << (ints0[i] + ints1[i]) << "\n";
   }
}

// int vectors
void example2(const STRICT_CPP_NAMESPACE::some_ints_A& ints0, const std::vector<int>& ints1) {
   std::cout << "\nexample2:\n";

   for (STRICT_CPP_NAMESPACE::size_t i = 0ULL; i < 5; i++) {
      std::cout << (ints0[i] + ints1[i]) << "\n";
   }
}

int main() {
   // Both some_string_A and some_string_B have the same encapsulated std::string type.
   const STRICT_CPP_NAMESPACE::some_string_A string0 = "some simple string A";
   const STRICT_CPP_NAMESPACE::some_string_B string1 = "some simple string B";
   const std::string                         string2 = "I'm ambiguous, but only implicitly!";

   example0(string0);
   example0(string1);

   // Ambiguous due to implicit conversion:
   //
   // example0(string2);
   //

   // Not ambiguous due to explicitly calling the some_string_A constructor:
   example0(STRICT_CPP_NAMESPACE::some_string_A(string2));

   // Both some_ints_A and some_ints_B have the same encapsulated std::vector<int> type.
   STRICT_CPP_NAMESPACE::some_ints_A ints0 = {};
   STRICT_CPP_NAMESPACE::some_ints_B ints1 = {};
   std::vector<int>                  ints2 = {};

   ints0.value.emplace_back(0);
   ints0.value.emplace_back(1);
   ints0.value.emplace_back(2);
   ints0.value.emplace_back(3);
   ints0.value.emplace_back(4);

   ints1.value.emplace_back(5);
   ints1.value.emplace_back(6);
   ints1.value.emplace_back(7);
   ints1.value.emplace_back(8);
   ints1.value.emplace_back(9);

   ints2.emplace_back(10);
   ints2.emplace_back(11);
   ints2.emplace_back(12);
   ints2.emplace_back(13);
   ints2.emplace_back(14);

   example1(ints0);
   example1(ints1);

   // Ambiguous due to implicit conversion:
   //
   // example1(ints2);
   //

   // Implicit conversion to encapsulated type for second parameter:
   example2(ints0, ints2);

   // Ambiguous due to only the second parameter qualifying implicitly for both, but first being implicitly ambiguous:
   //
   // example2(ints1, ints2);
   //

   // Members of the encapsulated type can be accessed using the indirect member access operator (->):
   const int results = ints0->at(2) + ints1->at(4);
   std::cout << "\nResults: " << results << "\n";

   // Subscript operator is enabled if the encapsulated type also has a subscript operator ([]):
   std::cout << "\nNumber: " << ints2[3] << "\n";

   return 0;
}
