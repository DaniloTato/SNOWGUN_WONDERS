// GameObjectDescriptor.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include <variant>

struct GameObjectDescriptor {

  struct Value {
    using Object = std::unordered_map<std::string, Value>;

    std::variant<std::monostate, int, float, double, bool, std::string, Object>
        data;

    Value() = default;
    Value(int v) : data(v) {}
    Value(float v) : data(v) {}
    Value(double v) : data(v) {}
    Value(bool v) : data(v) {}
    Value(std::string v) : data(std::move(v)) {}
    Value(Object v) : data(std::move(v)) {}
  };

  std::unordered_map<std::string, Value> fields;

  static Value describeVector2f(const sf::Vector2f &v);
};