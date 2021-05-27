#include <array>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <vector>
#include <utility>

class Serializable; // Forward declaration.

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

using Numbers         = std::vector<Number>;
using Texts           = std::vector<Text>;
using NumberSequences = std::vector<NumberSequence>;
using Serializables   = std::vector<Serializable>;

template<typename Type>
[[nodiscard]] static std::string serialize(const Type& serializable) {
  return serializable.serialize();
}

[[nodiscard]] static std::string serialize(const Serializables& v) {
  std::string result;

  for (const auto& s : v) {
    result += '<' + serialize(s) + '>';
  }

  return result;
}

class Serializable {
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

  std::shared_ptr<const Concept> model_;

public:
  Serializable() = default;

  template<typename Type>
  explicit Serializable(Type&& serializable)
    : model_{std::make_shared<Model<std::decay_t<Type>>>(std::forward<Type>(serializable))} {
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

  Serializable o1{Number{17}};
  Serializable o2{Text{"Hello"}};
  Serializable o3{NumberSequence{5, 4, 3, 2, 1, 0}};

  std::cout << serialize(o1) << '\n';
  std::cout << serialize(o2) << '\n';
  std::cout << serialize(o3) << '\n';

  Serializables v;
  v.emplace_back(Number{42});
  v.emplace_back(Text{"Hi"});
  v.emplace_back(NumberSequence{0, 1, 1, 2, 3, 5});
  v.emplace_back(Number{17});
  v.emplace_back(Text{"There"});
  v.emplace_back(Serializables{v});
  std::cout << serialize(v) << '\n';
}

// Compiler Explorer: https://www.godbolt.org/z/T7YTfc1ab