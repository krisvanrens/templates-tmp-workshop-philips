#include <type_traits>

template<typename Type>
struct A {};
 
template<typename Type>
struct B {};

template<typename Type>  // General case.
struct is_typeA
   : std::false_type {}; // Provides bool 'value'.

template<typename Type>  // Full specialization.
struct is_typeA<A<Type>>
   : std::true_type {};  // Provides bool 'value

int main() {
  static_assert(is_typeA<A<int>>::value);   // OK
  static_assert(is_typeA<A<float>>::value); // OK
  static_assert(is_typeA<B<int>>::value);   // FAIL
  static_assert(is_typeA<int>::value);      // FAIL
}

// Compiler Explorer: https://www.godbolt.org/z/a7YE98ETq