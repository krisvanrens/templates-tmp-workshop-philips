#include <array>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <type_traits>
#include <utility>

struct Number {
  const long data_;

  [[nodiscard]] std::string serialize() const {
    return std::to_string(data_);
  }
};

struct Text {
  const std::string data_;

  [[nodiscard]] std::string serialize() const {
    return data_;
  }
};

struct NumberSequence {
  const std::array<Number, 6> data_;

  [[nodiscard]] std::string serialize() const {
    return std::accumulate(std::next(data_.begin()), data_.end(), data_[0].serialize(),
                           [](auto a, auto b) { return std::move(a) + '-' + b.serialize(); });
  }
};

template<typename Type>
[[nodiscard]] static std::string serialize(const Type& serializable) {
  return serializable.serialize();
}

class Object {
  struct Concept {
    virtual ~Concept() = default;
    virtual std::string serialize() const = 0;
  };

  template<typename Type>
  struct Model final : Concept {
    const Type data_;

    template<typename OtherType = Type>
    explicit Model(OtherType&& data)
      : data_{std::forward<OtherType>(data)} {
    }

    std::string serialize() const override {
      return ::serialize(data_);
    }
  };

  std::unique_ptr<const Concept> model_;

public:
  template<typename Type>
  explicit Object(Type&& object)
    : model_{std::make_unique<Model<std::decay_t<Type>>>(std::forward<Type>(object))} {
  }

  [[nodiscard]] std::string serialize() const {
    return model_->serialize();
  }
};

int main() {
  Number n{42};
  std::cout << serialize(n) << '\n';

  Text t{"Hi"};
  std::cout << serialize(t) << '\n';

  NumberSequence s{0, 1, 1, 2, 3, 5};
  std::cout << serialize(s) << '\n';

  Object o1{Number{17}};
  Object o2{Text{"Hello"}};
  Object o3{NumberSequence{5, 4, 3, 2, 1, 0}};

  std::cout << serialize(o1) << '\n';
  std::cout << serialize(o2) << '\n';
  std::cout << serialize(o3) << '\n';
}

// Compiler Explorer: https://www.godbolt.org/z/bbjK51ej4