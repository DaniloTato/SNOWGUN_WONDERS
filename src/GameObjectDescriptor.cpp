#include "GameObjectDescriptor.hpp"

GameObjectDescriptor::Value
GameObjectDescriptor::describeVector2f(const sf::Vector2f &v) {
  Value::Object obj;
  obj["x"] = v.x;
  obj["y"] = v.y;
  return obj;
}