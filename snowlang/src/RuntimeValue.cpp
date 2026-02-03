#include "RuntimeValue.hpp"
#include "Cell.hpp"

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
            return "[list]";
          return "<List>(" + std::to_string(val.size()) + ")";
        }

        else if constexpr (std::is_same_v<T, RuntimeValue::Lambda>) {
          if (!showDataTypes)
            return "<lambda>";
          return "<Lambda>: " + std::to_string(val.instance->id);
        }

        else if constexpr (std::is_same_v<T, bool>) {
          const std::string data = val ? "true" : "false";
          return showDataTypes ? "<Bool>: " + data : data;
        }

        else if constexpr (std::is_same_v<T, std::monostate>) {
          return showDataTypes ? "<NULL>" : "null";
        }

        else if constexpr (std::is_same_v<T, ObjectRef>) {
          return showDataTypes ? "<Object>" : "<object>";
        }

        else if constexpr (std::is_same_v<T, CellPtr>) {
          return showDataTypes ? "<Address> -> " + val->getCellValue().toString(true) : "<CellRef>";
        }

        else {
          return "<Unknown RuntimeValue>";
        }
      },
      data);
}