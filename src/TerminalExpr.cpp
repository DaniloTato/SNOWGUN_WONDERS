#include "TerminalExpr.hpp"
#include "Terminal.hpp"
#include <stdexcept>

TerminalCommands::RuntimeValue ParameterExpr::eval(Terminal &terminal) {
  return {.type = TerminalCommands::DataType::Parameter, .data = value};
}

TerminalCommands::RuntimeValue CommandExpr::eval(Terminal &terminal) {

  auto &commandMap = Terminal::getCommandMap();
  auto it = commandMap.find(name);

  if (it == commandMap.end()) {
    throw std::runtime_error("Unknown command: " + name);
  }

  TerminalCommands::Context ctx;
  ctx.terminal = &terminal;

  TerminalCommands::ArgsVector evaluatedArgs;
  evaluatedArgs.reserve(args.size());

  for (auto &arg : args) {
    evaluatedArgs.push_back(arg->eval(terminal));
  }

  return it->second.execute(&terminal, ctx, evaluatedArgs);
}

TerminalCommands::RuntimeValue DerefExpr::eval(Terminal &terminal) {

  TerminalCommands::RuntimeValue value = inner->eval(terminal);

  if (std::holds_alternative<
          TerminalCommands::RuntimeValue::GameObjectReference>(value.data)) {
    auto ref = std::get<TerminalCommands::RuntimeValue::GameObjectReference>(
        value.data);
    return TerminalCommands::makeObjectView(*ref);
  }

  if (std::holds_alternative<TerminalCommands::RuntimeValue::RuntimeVariable>(
          value.data)) {
    auto ref =
        std::get<TerminalCommands::RuntimeValue::RuntimeVariable>(value.data);
    return ref->value;
  }

  throw std::runtime_error("Cannot dereference non-reference");

  return {};
}

TerminalCommands::RuntimeValue MemberExpr::eval(Terminal &terminal) {

  auto baseValue = base->eval(terminal);

  if (baseValue.type != TerminalCommands::DataType::Object) {
    throw std::runtime_error("Cannot access member of non-object");
  }

  auto &obj = std::get<TerminalCommands::RuntimeValue::Object>(baseValue.data);

  auto it = obj.find(member);
  if (it == obj.end()) {
    throw std::runtime_error("No such member: " + member);
  }

  return it->second;
}

TerminalCommands::RuntimeValue AssignmentExpr::eval(Terminal &terminal) {
  auto left = lhs->eval(terminal);

  if (left.type != TerminalCommands::DataType::Variable) {
    throw std::runtime_error("Left-hand side is not assignable");
  }

  auto *var =
      std::get<TerminalCommands::RuntimeValue::RuntimeVariable>(left.data);

  if (var->isConst) {
    throw std::runtime_error("Cannot assign to const variable");
  }

  auto value = rhs->eval(terminal);
  var->value = value;

  return value;
}

TerminalCommands::RuntimeValue StringExpr::eval(Terminal &) {
  return {.type = TerminalCommands::DataType::Parameter, .data = value};
}

TerminalCommands::RuntimeValue BlockExpr::eval(Terminal &) {
  return {.type = TerminalCommands::DataType::Expression, .data = inner};
}