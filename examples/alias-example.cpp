#include "strict-cpp.hpp"

#include <iostream>
#include <string>
#include <vector>

struct struct_with_members {
		int foo;
		int bar;

		int get_foo() const { return this->foo; }

		int get_bar() const { return this->bar; }

		constexpr int& operator[](int) noexcept { return this->foo; }

		constexpr const int& operator[](const int) const noexcept { return this->foo; }
};

STRICT_CPP_DEFINE_ALIAS_TYPE(some_string_A, std::string);
STRICT_CPP_DEFINE_ALIAS_TYPE(some_string_B, std::string);
STRICT_CPP_DEFINE_ALIAS_TYPE(some_ints_A, std::vector<int>);
STRICT_CPP_DEFINE_ALIAS_TYPE(some_ints_B, std::vector<int>);
STRICT_CPP_DEFINE_ALIAS_TYPE(encapsulated_int_ptr, int*);
STRICT_CPP_DEFINE_ALIAS_TYPE(encapsulated_int, int);
STRICT_CPP_DEFINE_ALIAS_TYPE(some_struct_alias, struct_with_members);
STRICT_CPP_DEFINE_ALIAS_TYPE(some_struct_ptr_alias, struct_with_members*);

// some_string_A
void example0(const STRICT_CPP_NAMESPACE::some_string_A& string) { std::cout << "\nexample0:\n"
																									  << string.value << "\n"; }

// some_string_B
void example0(const STRICT_CPP_NAMESPACE::some_string_B& string) { std::cout << "\nexample0:\n"
																									  << string.value << "\n"; }

// some_ints_A
void example1(const STRICT_CPP_NAMESPACE::some_ints_A& ints) {
	std::cout << "\nexample1:\n";

	for (const int i : ints) std::cout << i << "\n";
}

// some_ints_B
void example1(const STRICT_CPP_NAMESPACE::some_ints_B& ints) {
	std::cout << "\nexample1:\n";

	for (const int i : ints) std::cout << i << "\n";
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
	const std::string									string2 = "I'm ambiguous!";

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
	std::vector<int>						 ints2 = {};

	// To access members, you can use .value explicitly:
	ints0.value.emplace_back(0);
	ints0.value.emplace_back(1);
	ints0.value.emplace_back(2);
	ints0.value.emplace_back(3);
	ints0.value.emplace_back(4);

	// Alternatively, the indirect member access operator (->) can be used:
	ints1->emplace_back(5);
	ints1->emplace_back(6);
	ints1->emplace_back(7);
	ints1->emplace_back(8);
	ints1->emplace_back(9);

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

	// Ambiguous due to only the second parameter qualifying implicitly for both, but first being implicitly ambiguous for only one:
	//
	// example2(ints1, ints2);
	//

	// Members of the encapsulated type can be accessed using the indirect member access operator (->):
	const int results = ints0->at(2) + ints1->at(4);
	std::cout << "\nResults: " << results << "\n";

	// Subscript operator is enabled if the encapsulated type also has a subscript operator ([]):
	std::cout << "\nNumber: " << ints1[3] << "\n";

	const STRICT_CPP_NAMESPACE::some_struct_alias some_struct = struct_with_members(5, 6);
	std::cout << "The sum is: " << some_struct[0] << "\n";

	// An alias type can also encapsulate pointers:
	STRICT_CPP_NAMESPACE::encapsulated_int_ptr int_ptr = new int[5](1, 2, 3, 4, 5);

	int_ptr[0] = 1234;
	int_ptr[3] = 5678;

	std::cout << "\n"
				 << int_ptr[0] << ", "
				 << int_ptr[1] << ", "
				 << int_ptr[2] << ", "
				 << int_ptr[3] << ", "
				 << int_ptr[4];

	const STRICT_CPP_NAMESPACE::some_struct_ptr_alias some_struct_ptr = new struct_with_members(5, 6);

	std::cout << "\n\n";
	std::cout << "foo is: " << some_struct_ptr->get_foo() << "\n";
	std::cout << "bar is: " << some_struct_ptr->get_bar() << "\n";

	// the delete keyword can still be used to free memory
	// as the encapsulated type is implicitly returned.
	delete(int_ptr);
	delete(some_struct_ptr);

	// Alias types also support formatters/stringification:
	const STRICT_CPP_NAMESPACE::encapsulated_int encapsulated_int{678};
	const STRICT_CPP_NAMESPACE::some_ints_A		encapsulated_ints_vector{5, 6, 7, 8};

	// An int is easily formatted by just displaying the number:
	std::cout << std::format("\n\nthis is a formatted message with the number {}!", encapsulated_int);

	// But an encapsulated type that doesn't have formatting support gets just the type's name displayed instead:
	std::cout << std::format("\n\nthis is a formatted message with the type's name {}!", encapsulated_ints_vector);

	// Simple to_string method:
	std::cout << "\n\nThe value is: " << encapsulated_int.to_string();

	// Alias types can support for-loops if the encapsulated type has the required functions:
	std::cout << "\n\nLoops:\n";

	for (const int& value : encapsulated_ints_vector) {
		std::cout << value << '\n';
	}

	return 0;
}
