#include "GameObjectExposure.hpp"
#include "GameCamera.hpp"
#include "GameState.hpp"
#include "SnowlangInstance.hpp"

namespace GameObjectExposure {

std::function<void(std::string)> logger = [](const std::string &msg) {
  Snowlang::SnowlangInstance::getLatestSnowlangInstance().io.writeLn(msg);
};

Value::Object Descriptor::describeVector2f(sf::Vector2f &v) {

  auto desc = std::make_shared<Descriptor>();

  desc->fields["x"] = makePublicField(v.x);
  desc->fields["y"] = makePublicField(v.y);

  return desc;
}

Value::Object Descriptor::describeActiveCameraList() {
  auto desc = std::make_shared<Descriptor>();

  desc->fields["main"] = makeConstFieldValue([]() -> Value {
    return Value{.value =
                     describeCamera(GameState::getInstance().getMainCamera())};
  });

  desc->fields["ui"] = makeConstFieldValue([]() -> Value {
    return Value{.value =
                     describeCamera(GameState::getInstance().getUiCamera())};
  });

  desc->fields["terminal"] = makeConstFieldValue([]() -> Value {
    return Value{
        .value = describeCamera(GameState::getInstance().getTerminalCamera())};
  });

  return desc;
}

Value::Object Descriptor::describeCamera(GameCamera *cam) {
  static std::unordered_map<GameCamera *, Value::Object> cache;

  if (auto it = cache.find(cam); it != cache.end())
    return it->second;

  auto desc = std::make_shared<Descriptor>();

  desc->fields["pos"] = makeFieldValue(
      [cam]() {
        auto fx = makeConstField<float>(
            [cam]() { return cam->getDesiredPosition().x; });

        auto fy = makeConstField<float>(
            [cam]() { return cam->getDesiredPosition().y; });

        std::array<Field::Ptr, 2> fields{fx, fy};
        return describeRefList(fields);
      },

      [cam](const Value &v) {
        auto list = fromValue<Value::List>(v);
        float index0 = cam->getDesiredPosition().x;
        if (list.size() >= 1) {
          index0 = fromValue<float>(list[0]);
        }

        float index1 = cam->getDesiredPosition().y;
        if (list.size() >= 2) {
          index1 = fromValue<float>(list[1]);
        }

        cam->goTo({index0, index1});
      });

  desc->fields["zoom"] =
      makeField<float>([cam]() { return cam->getDesiredZoom(); },
                       [cam](float v) { cam->zoomTo(v); });

  cache[cam] = desc;
  return desc;
}

Value Descriptor::describeRefList(std::span<Field::Ptr> fields) {
  Value::List list;

  for (auto &field : fields) {
    list.push_back(Value{.value = field});
  }

  return Value{.value = list};
}

} // namespace GameObjectExposure