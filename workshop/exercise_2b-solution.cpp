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

template<typename Type>
inline static constexpr bool is_typeA_v = is_typeA<Type>::value;

int main() {
  static_assert(is_typeA_v<A<int>>);   // OK
  static_assert(is_typeA_v<A<float>>); // OK
  static_assert(is_typeA_v<B<int>>);   // FAIL
  static_assert(is_typeA_v<int>);      // FAIL
}

// Compiler Explorer: https://www.godbolt.org/z/ze5K4qns1