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

  TerminalCommands::ArgsVector evaluatedArgs;
  evaluatedArgs.reserve(args.size());

  for (auto &arg : args) {
    evaluatedArgs.push_back(arg->eval(terminal));
  }

  return it->second.function(&terminal, it->second.signature, evaluatedArgs);
}

TerminalCommands::RuntimeValue DerefExpr::eval(Terminal &terminal) {

  TerminalCommands::RuntimeValue value = inner->eval(terminal);

  if (std::holds_alternative<
          TerminalCommands::RuntimeValue::GameObjectReference>(value.data)) {
    auto ref = std::get<TerminalCommands::RuntimeValue::GameObjectReference>(
        value.data);
    return TerminalInterpreter::makeObjectView(*ref);
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

TerminalCommands::RuntimeValue NumberExpr::eval(Terminal &) {
  return {.type = TerminalCommands::DataType::Number, .data = value};
}

TerminalCommands::RuntimeValue BlockExpr::eval(Terminal &) {
  return {.type = TerminalCommands::DataType::Expression, .data = inner};
}

TerminalCommands::RuntimeValue ValueExpr::eval(Terminal &terminal) {
  TerminalCommands::RuntimeValue value = inner->eval(terminal);
  if (std::holds_alternative<TerminalCommands::RuntimeValue::Parameter>(
          value.data)) {

    auto var_name =
        std::get<TerminalCommands::RuntimeValue::Parameter>(value.data);
    Variable *var = terminal.memory.getVariable(var_name);
    return var->value;
  }

  throw std::runtime_error("Cannot get value of a non-parameter");

  return {};
}

bool CompareExpr::runtimeEqual(const TerminalCommands::RuntimeValue &a,
                               const TerminalCommands::RuntimeValue &b) {

  using namespace TerminalCommands;

  if (a.type != b.type)
    return false;

  switch (a.type) {

  case DataType::Number:
    return std::get<RuntimeValue::Number>(a.data) ==
           std::get<RuntimeValue::Number>(b.data);

  case DataType::Parameter:
    return std::get<RuntimeValue::Parameter>(a.data) ==
           std::get<RuntimeValue::Parameter>(b.data);

  case DataType::Variable:
    return std::get<RuntimeValue::RuntimeVariable>(a.data) ==
           std::get<RuntimeValue::RuntimeVariable>(b.data);

  case DataType::Object: {
    const auto &lhs = std::get<RuntimeValue::Object>(a.data);
    const auto &rhs = std::get<RuntimeValue::Object>(b.data);

    if (lhs.size() != rhs.size())
      return false;

    for (const auto &[key, lval] : lhs) {
      auto it = rhs.find(key);
      if (it == rhs.end())
        return false;

      if (!runtimeEqual(lval, it->second))
        return false;
    }
    return true;
  }

  default:
    return false;
  }
}

TerminalCommands::RuntimeValue CompareExpr::eval(Terminal &terminal) {
  auto l = lhs->eval(terminal);
  auto r = rhs->eval(terminal);

  if (op == CompareOp::Equal || op == CompareOp::NotEqual) {
    bool eq = runtimeEqual(l, r);
    bool result = (op == CompareOp::Equal) ? eq : !eq;

    return {.type = TerminalCommands::DataType::Bool, .data = result};
  }

  auto getNum = [](const TerminalCommands::RuntimeValue &v) -> float {
    if (v.type != TerminalCommands::DataType::Number)
      throw std::runtime_error("Comparison requires numbers");
    return std::get<TerminalCommands::RuntimeValue::Number>(v.data);
  };

  float a = getNum(l);
  float b = getNum(r);

  bool result = false;

  switch (op) {
  case CompareOp::Greater:
    result = a > b;
    break;
  case CompareOp::Less:
    result = a < b;
    break;
  case CompareOp::GreaterEqual:
    result = a >= b;
    break;
  case CompareOp::LessEqual:
    result = a <= b;
    break;
  default:
    throw std::runtime_error("Invalid comparison operator");
  }

  return {TerminalCommands::DataType::Bool, result};
}

TerminalCommands::RuntimeValue MathExpr::eval(Terminal &terminal) {
  auto l = lhs->eval(terminal);
  auto r = rhs->eval(terminal);

  if (l.type != TerminalCommands::DataType::Number ||
      l.type != TerminalCommands::DataType::Number) {
    throw std::runtime_error("Math operators require numeric operands");
  }

  auto a = std::get<TerminalCommands::RuntimeValue::Number>(l.data);
  auto b = std::get<TerminalCommands::RuntimeValue::Number>(r.data);

  switch (op) {
  case MathOp::Add:
    return {TerminalCommands::DataType::Number, a + b};
  case MathOp::Sub:
    return {TerminalCommands::DataType::Number, a - b};
  case MathOp::Mul:
    return {TerminalCommands::DataType::Number, a * b};
  case MathOp::Div:
    if (b == 0.0) {
      throw std::runtime_error("Division by zero");
    }
    return {TerminalCommands::DataType::Number, a / b};
  }

  return {};
}