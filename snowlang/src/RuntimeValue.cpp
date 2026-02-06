#include "RuntimeValue.hpp"
#include "Cell.hpp"

namespace Snowlang {

std::string RuntimeValue::toString(bool showDataTypes) const {
  return std::visit(
      [showDataTypes](auto &&val) -> std::string {
        using T = std::decay_t<decltype(val)>;

        if constexpr (std::is_same_v<T, float>) {
          const std::string data = std::to_string(val);
          return showDataTypes ? "<Number>: " + data : data;
        }

        else if constexpr (std::is_same_v<T, std::string>) {
          return showDataTypes ? "<String>: " + val : val;
        }

        else if constexpr (std::is_same_v<T, RuntimeValue::List>) {
          if (!showDataTypes)
            return "<List>";
          return "<List>(" + std::to_string(val.size()) + ")";
        }

        else if constexpr (std::is_same_v<T, RuntimeValue::Lambda>) {
          if (!showDataTypes)
            return "<Lambda>";
          return "<Lambda>: " + std::to_string(val.instance->id);
        }

        else if constexpr (std::is_same_v<T, bool>) {
          const std::string data = val ? "true" : "false";
          return showDataTypes ? "<Bool>: " + data : data;
        }

        else if constexpr (std::is_same_v<T, std::monostate>) {
          return "<NULL>";
        }

        else if constexpr (std::is_same_v<T, ObjectRef>) {
          return "<Object>";
        }

        else if constexpr (std::is_same_v<T, CellPtr>) {
          return showDataTypes ? "<Address> -> " + val->getCellValue().toString(true) : "<CellRef>";
        }

        else if constexpr (std::is_same_v<T, GameObjectRef>) {
          return "<GameObjectRef>";
        }

        else {
          return "<Unknown RuntimeValue>";
        }
      },
      data);
}

bool RuntimeValue::isRef() const { return std::holds_alternative<GameObjectRef>(data); }

RuntimeValue RuntimeValue::readRef() const {
  if (auto *ref = std::get_if<GameObjectRef>(&data))
    return ref->getter();
  return *this;
}

void RuntimeValue::writeRef(const RuntimeValue &v) {
  if (auto *ref = std::get_if<GameObjectRef>(&data))
    ref->setter(v);
}

} // namespace Snowlang