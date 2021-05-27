#include <array>
#include <cstdio>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>

struct Noisy {
  Noisy()                        { puts(__PRETTY_FUNCTION__); }
  ~Noisy()                       { puts(__PRETTY_FUNCTION__); }
  Noisy(const Noisy&)            { puts(__PRETTY_FUNCTION__); }
  Noisy& operator=(const Noisy&) { puts(__PRETTY_FUNCTION__); return *this; }
  Noisy(Noisy&&)                 { puts(__PRETTY_FUNCTION__); }
  Noisy& operator=(Noisy&&)      { puts(__PRETTY_FUNCTION__); return *this; }
};

struct Number {
  const long data_;
};

struct Text {
  const std::string data_;
};

struct NumberSequence {
  const std::array<Number, 6> data_;
};

class Object {
  struct Concept {
    virtual ~Concept() = default;
  };

  template<typename Type>
  struct Model final : Concept {
    const Type data_;

    template<typename OtherType = Type>
    explicit Model(OtherType&& data)
      : data_{std::forward<OtherType>(data)} {
    }
  };

  std::unique_ptr<const Concept> model_;

public:
  template<typename Type>
  explicit Object(Type&& object)
    : model_{std::make_unique<Model<std::decay_t<Type>>>(std::forward<Type>(object))} {
  }
};

int main() {
  Object o1{Number{17}};
  Object o2{Text{"Hello"}};
  Object o3{NumberSequence{5, 4, 3, 2, 1, 0}};
  Object o4{Noisy{}};
}

// Compiler Explorer: https://www.godbolt.org/z/esGMTvrso