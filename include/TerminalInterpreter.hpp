#pragma once

#include "TerminalCommands.hpp"
#include <string>

class TerminalInterpreter {
public:
  std::string toString(const TerminalCommands::RuntimeValue &value);
  [[nodiscard]] TerminalCommands::DataType
  stringToDataType(std::string_view strvw);

private:
  std::string nullToString();
  std::string stringToString(const std::string &s);
  const TerminalCommands::RuntimeValue &
  extractVariableValue(const TerminalCommands::RuntimeValue &varValue);
  std::string
  referenceToString(TerminalCommands::RuntimeValue::GameObjectReference ref);
  std::string
  referenceToString(TerminalCommands::RuntimeValue::RuntimeVariable ref);
  std::string
  referenceToString(const TerminalCommands::RuntimeValue::Expression &ref);
  std::string objectToString(const TerminalCommands::RuntimeValue::Object &obj);

  const std::unordered_map<std::string, TerminalCommands::DataType>
      stringToDataTypeMap = {
          {"_null", TerminalCommands::DataType::Null},
          {"_gameref", TerminalCommands::DataType::GameObjectReference},
          {"_var", TerminalCommands::DataType::Variable},
          {"_obj", TerminalCommands::DataType::Object},
          {"_param", TerminalCommands::DataType::Parameter},
          {"_type", TerminalCommands::DataType::Type},
          {"_msg", TerminalCommands::DataType::Message},
          {"_num", TerminalCommands::DataType::Number},
          {"_list", TerminalCommands::DataType::List},
  };
};