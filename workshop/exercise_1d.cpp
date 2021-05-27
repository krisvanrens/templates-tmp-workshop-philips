#include <cstdio>
#include <type_traits>
#include <utility>

template<typename Type>
concept NonBooleanIntegral = std::is_integral_v<Type> && !std::is_same_v<Type, bool>;

template<NonBooleanIntegral Type>
class Fraction {
  Type numerator_   = {};
  Type denominator_ = 1;
 
public:
  Fraction(Type numerator, Type denominator)
    : numerator_{numerator},
      denominator_{denominator} {
  }

  // Perfect forwarding constructor.
  template<typename OtherType,
           std::enable_if_t<std::is_convertible_v<Type, OtherType>, bool> = true>
  explicit Fraction(OtherType &&other)
    : numerator_{static_cast<Type>(std::forward<OtherType>(other))} {
  }
 
  Type numerator()   { return numerator_;   }
  Type denominator() { return denominator_; }
 
  double real() {
    return static_cast<double>(numerator_) / denominator_;
  }
};

int main() {
  Fraction<int>           x{22, 7};
  Fraction<unsigned long> y{355U, 113U};
  Fraction<unsigned int>  z{3.1};

  auto c{x}; // Erroneously calls forwarding ctor.
  
  printf("x = %.8lf\n", x.real());
  printf("y = %.8lf\n", y.real());
  printf("z = %.8lf\n", z.real());
}

// Compiler Explorer: https://www.godbolt.org/z/ae9dTTEac