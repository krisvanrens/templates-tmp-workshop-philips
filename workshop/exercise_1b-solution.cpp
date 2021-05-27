#include <cstdio>
#include <type_traits>

template<typename Type>
concept NonBooleanIntegral = std::is_integral_v<Type> && !std::is_same_v<Type, bool>;

template<NonBooleanIntegral Type>
class Fraction {
  Type numerator_   = {};
  Type denominator_ = {};
 
public:
  Fraction(Type numerator, Type denominator)
    : numerator_{numerator},
      denominator_{denominator} {
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
  
  printf("x = %.8lf\n", x.real());
  printf("y = %.8lf\n", y.real());
}

// Compiler Explorer: https://www.godbolt.org/z/bxM3rv7Y3