#include "TerminalInterpreter.hpp"
#include "TerminalMemory.hpp"

#include <sstream>

const TerminalCommands::RuntimeValue &TerminalInterpreter::extractVariableValue(
    const TerminalCommands::RuntimeValue &varValue) {
  if (varValue.type != TerminalCommands::DataType::Variable) {
    throw std::runtime_error(
        "extractVariableValue: RuntimeValue is not a Variable");
  }

  auto varPtr =
      std::get<TerminalCommands::RuntimeValue::RuntimeVariable>(varValue.data);
  if (!varPtr) {
    throw std::runtime_error("extractVariableValue: Variable pointer is null");
  }
  return varPtr->value;
}

std::string
TerminalInterpreter::toString(const TerminalCommands::RuntimeValue &value) {
  switch (value.type) {
  case TerminalCommands::DataType::Null:
    return nullToString();

  case TerminalCommands::DataType::Parameter:
  case TerminalCommands::DataType::Message:
    return stringToString(std::get<std::string>(value.data));

  case TerminalCommands::DataType::GameObjectReference:
    return "__gameref__ " +
           referenceToString(
               std::get<TerminalCommands::RuntimeValue::GameObjectReference>(
                   value.data));

  case TerminalCommands::DataType::Variable:
    return "__variable__ " +
           referenceToString(
               std::get<TerminalCommands::RuntimeValue::RuntimeVariable>(
                   value.data));

  case TerminalCommands::DataType::Object:
    return "__object__<ln>" +
           objectToString(
               std::get<TerminalCommands::RuntimeValue::Object>(value.data));

  case TerminalCommands::DataType::Number:
    return "__number__<ln>" +
           std::to_string(
               std::get<TerminalCommands::RuntimeValue::Number>(value.data));

  case TerminalCommands::DataType::Type:
    return "__type__";

  case TerminalCommands::DataType::Function:
    return "__function__";

  case TerminalCommands::DataType::Expression:
    return "__expression__ " +
           referenceToString(
               std::get<TerminalCommands::RuntimeValue::Expression>(
                   value.data));
    ;

  case TerminalCommands::DataType::List: {
    std::string out = "__list__";
    TerminalCommands::RuntimeValue::List list =
        std::get<TerminalCommands::RuntimeValue::List>(value.data);
    for (auto &i : list) {
      out += "<ln>";
      out += "---" + toString(i);
    }
    return out;
  }

  default:
    return "__unknown__";
  }
}

TerminalCommands::DataType
TerminalInterpreter::stringToDataType(std::string_view strvw) {
  std::string str{strvw};
  auto it = stringToDataTypeMap.find(str);

  if (it == stringToDataTypeMap.end()) {
    return TerminalCommands::DataType::Null;
  }

  return it->second;
}

std::string TerminalInterpreter::nullToString() { return "null"; }

std::string TerminalInterpreter::stringToString(const std::string &s) {
  return s;
}

std::string TerminalInterpreter::referenceToString(
    TerminalCommands::RuntimeValue::GameObjectReference ref) {
  std::ostringstream oss;
  oss << "0x" << std::hex << ref;
  return oss.str();
}

std::string TerminalInterpreter::referenceToString(
    TerminalCommands::RuntimeValue::RuntimeVariable ref) {
  std::ostringstream oss;
  oss << "0x" << std::hex << ref;
  return oss.str();
}

std::string TerminalInterpreter::referenceToString(
    const TerminalCommands::RuntimeValue::Expression &ref) {
  std::ostringstream oss;
  oss << "0x" << std::hex << ref;
  return oss.str();
}

std::string TerminalInterpreter::objectToString(
    const TerminalCommands::RuntimeValue::Object &obj) {

  std::ostringstream oss;
  oss << "{\n";

  for (const auto &[key, value] : obj) {
    std::string valueStr = toString(value);

    std::ostringstream indented;
    std::istringstream iss(valueStr);
    std::string line;
    bool firstLine = true;

    while (std::getline(iss, line)) {
      if (!firstLine)
        indented << "\n";
      indented << "$   " << line;
      firstLine = false;
    }

    oss << "$   " << key << ":" << "\n" << indented.str() << ",\n";
  }

  oss << "}";
  return oss.str();
}