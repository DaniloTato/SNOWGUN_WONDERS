#pragma once

#include "TerminalCommands.hpp"
#include <string_view>
#include <unordered_map>

using RuntimeScope = std::unordered_map<std::string, Variable>;

class TerminalMemory;

struct Variable {
  TerminalCommands::RuntimeValue value;
  bool isConst = false;
  bool isReserved = false;
};

struct Function {
  std::shared_ptr<Expr> body;
  TerminalCommands::RuntimeValue::List parameters;
  TerminalMemory *closure;
};

class TerminalMemory {
public:
  TerminalMemory();
  bool declareVariable(std::string_view name,
                       TerminalCommands::RuntimeValue value,
                       bool isConst = false);
  bool setVariable(std::string_view name, TerminalCommands::RuntimeValue value);
  [[nodiscard]] Variable *getVariable(std::string_view name);

  void pushScope();
  void popScope();

private:
  std::vector<RuntimeScope> scopes;
};