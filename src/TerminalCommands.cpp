#include "TerminalCommands.hpp"
#include "Terminal.hpp"
#include "TerminalExpr.hpp"

#include <iostream>
#include <string>

namespace TerminalCommands {

namespace {
TerminalCommands::RuntimeValue echoCommand(Terminal *owner, Context &ctx,
                                           const ArgsVector &args) {

  if (args.empty()) {
    return {.type = DataType::Message,
            .data = std::string("[echo] command received no params.")};
  }

  std::string out;

  for (size_t i = 0; i < args.size(); ++i) {
    if (i)
      out += " ";

    out += owner->getInterpreter().toString(args[i]);
  }

  return {.type = DataType::Parameter, .data = out};
}

TerminalCommands::RuntimeValue helpCommand(Terminal *owner, Context &ctx,
                                           const ArgsVector &args) {

  if (!args.empty()) {
    return {.type = DataType::Message,
            .data = std::string("[HELP] command receives no args")};
  }

  std::string out;
  out += "<color=green>Available commands:</color>\n";

  for (const auto &[name, cmd] : Terminal::getCommandMap()) {
    out += "  <color=yellow>" + name + "</color>: " + cmd.description + "\n";
  }

  return {.type = DataType::Message, .data = out};
}

TerminalCommands::RuntimeValue getCommand(Terminal *owner, Context &ctx,
                                          const ArgsVector &args) {

  if (args.size() != 1) {
    return {.type = DataType::Message,
            .data = std::string("[GET] command only accepts 1 arg.")};
  }

  const RuntimeValue &name = args[0];

  if (name.type != DataType::Parameter) {
    return {.type = DataType::Message,
            .data = std::string("[GET] command only accepts raw parameters.")};
  }

  auto &str = std::get<std::string>(name.data);

  Variable *ref = owner->getMemory().getVariable(str);

  if (!ref) {
    return {.type = DataType::Message,
            .data = std::string("[GET] unknown reference: ") + str};
  }

  return {.type = DataType::Variable, .data = ref};
}

TerminalCommands::RuntimeValue newCommand(Terminal *owner, Context &ctx,
                                          const ArgsVector &args) {

  if (args.size() != 1) {
    return {.type = DataType::Message,
            .data = std::string("[NEW] command only accepts 1 arg.")};
  }

  const RuntimeValue &name = args[0];

  if (name.type != DataType::Parameter) {
    return {.type = DataType::Message,
            .data = std::string("[NEW] command only accepts raw parameters.")};
  }

  auto &str = std::get<std::string>(name.data);

  bool success = owner->getMemory().declareVariable(
      str, RuntimeValue{.type = DataType::Null, .data = std::monostate()});

  if (!success) {
    return {.type = DataType::Message,
            .data = std::string("[NEW] variable was previously declared")};
  }

  return {.type = DataType::Variable,
          .data = owner->getMemory().getVariable(str)};
}

TerminalCommands::RuntimeValue typeCommand(Terminal *owner, Context &ctx,
                                           const ArgsVector &args) {

  if (args.size() != 1) {
    return {.type = DataType::Message,
            .data = std::string("[TYPE] command only accepts 1 arg.")};
  }

  const RuntimeValue &strType = args[0];

  if (strType.type != DataType::Parameter) {
    return {.type = DataType::Message,
            .data = std::string("[TYPE] command only accepts raw parameters.")};
  }

  DataType type = owner->getInterpreter().stringToDataType(
      std::get<std::string>(strType.data));

  if (type == DataType::Null) {
    return {.type = DataType::Message,
            .data = std::string("[TYPE] invalid data type.")};
  }

  return {.type = DataType::Type, .data = type};
}

TerminalCommands::RuntimeValue numCommand(Terminal *owner, Context &ctx,
                                          const ArgsVector &args) {

  if (args.size() != 1) {
    return {.type = DataType::Message,
            .data = std::string("[VAL] requires strictly a single arg.")};
  }

  const RuntimeValue &strNum = args[0];

  if (strNum.type != DataType::Parameter) {
    return {.type = DataType::Message,
            .data = std::string("[VAL] command only accepts raw params.")};
  }

  const auto &param =
      std::get<TerminalCommands::RuntimeValue::Parameter>(strNum.data);

  try {
    float value = std::stof(param);
    return {.type = DataType::Number, .data = value};
  } catch (const std::exception &) {
    return {.type = DataType::Message,
            .data = std::string("[VAL] invalid number")};
  }

  return {.type = DataType::Message, .data = "msg"};
}

TerminalCommands::RuntimeValue listCommand(Terminal *owner, Context &ctx,
                                           const ArgsVector &args) {
  TerminalCommands::RuntimeValue::List list;
  list.reserve(args.size());

  for (const auto &arg : args) {
    list.push_back(arg);
  }

  return {.type = DataType::List, .data = std::move(list)};
}

TerminalCommands::RuntimeValue clearCommand(Terminal *owner, Context &ctx,
                                            const ArgsVector &args) {
  owner->clear();
  return {.type = DataType::Message, .data = ""};
}

TerminalCommands::RuntimeValue funCommand(Terminal *owner, Context &ctx,
                                          const ArgsVector &args) {

  if (args.size() != 2) {
    return {};
  }

  const RuntimeValue &paramsExpr = args[0];
  const RuntimeValue &bodyExpr = args[1];

  auto *fnRef = new Function;

  if (paramsExpr.type != TerminalCommands::DataType::List) {
    return {};
  }

  TerminalCommands::RuntimeValue::List list =
      std::get<TerminalCommands::RuntimeValue::List>(paramsExpr.data);

  for (RuntimeValue &val : list) {
    if (val.type != TerminalCommands::DataType::Parameter) {
      return {};
    }
  }
  fnRef->parameters = list;

  if (bodyExpr.type != TerminalCommands::DataType::Expression) {
    return {};
  }

  fnRef->body =
      std::get<TerminalCommands::RuntimeValue::Expression>(bodyExpr.data);
  fnRef->closure = &owner->getMemory();

  return {.type = DataType::Function, .data = fnRef};
}

TerminalCommands::RuntimeValue doCommand(Terminal *owner, Context &ctx,
                                         const ArgsVector &args) {

  if (args.empty()) {
    return {};
  }

  const RuntimeValue &fnVal = args[0];

  if (fnVal.type != DataType::Function) {
    throw std::runtime_error("[do] first argument is not a function");
  }

  Function *fn = std::get<RuntimeValue::FunctionPtr>(fnVal.data);

  if (args.size() - 1 != fn->parameters.size()) {
    throw std::runtime_error("[do] wrong number of arguments");
  }

  TerminalMemory *memory = fn->closure;

  memory->pushScope();

  for (size_t i = 0; i < fn->parameters.size(); ++i) {
    const RuntimeValue &param = fn->parameters[i];

    const auto &name = std::get<RuntimeValue::Parameter>(param.data);

    memory->declareVariable(name, args[i + 1], false);
  }

  RuntimeValue result = fn->body->eval(*owner);

  memory->popScope();

  return result;
}

} // namespace

void registerAll() {

  Terminal::registerCommand(
      {"help", helpCommand, "get list of all registered commmands"});
  Terminal::registerCommand(
      {"echo", echoCommand, "print the returned value of the input"});
  Terminal::registerCommand(
      {"get", getCommand, "returns a reference to a variable instance"});
  Terminal::registerCommand({"new", newCommand, "declares a new variable"});
  Terminal::registerCommand(
      {"type", typeCommand, "converts parameter into data type"});
  Terminal::registerCommand(
      {"num", numCommand, "converts parameter into number"});
  Terminal::registerCommand(
      {"list", listCommand, "groups parameters into a list"});
  Terminal::registerCommand(
      {"clear", clearCommand, "clears the console history"});
  Terminal::registerCommand(
      {"fun", funCommand, "creates a function reference"});
  Terminal::registerCommand({"do", doCommand, "executes a function"});
}

RuntimeValue makeObjStrRuntimeValue(const GameObjectDescriptor::Value &value) {
  using V = GameObjectDescriptor::Value;

  return std::visit(
      [&](auto &&v) -> RuntimeValue {
        using T = std::decay_t<decltype(v)>;

        if constexpr (std::is_same_v<T, std::monostate>) {
          return {DataType::Null, std::monostate{}};

        } else if constexpr (std::is_arithmetic_v<T>) {
          return {DataType::Parameter, std::to_string(v)};

        } else if constexpr (std::is_same_v<T, bool>) {
          return {DataType::Parameter, v ? "true" : "false"};

        } else if constexpr (std::is_same_v<T, std::string>) {
          return {DataType::Parameter, v};

        } else if constexpr (std::is_same_v<T, V::Object>) {
          RuntimeValue::Object obj;
          for (const auto &[k, child] : v) {
            obj[k] = makeObjStrRuntimeValue(child);
          }
          return {DataType::Object, std::move(obj)};
        }
      },
      value.data);
}

RuntimeValue makeObjectView(const GameObject &obj) {
  RuntimeValue::Object out;
  GameObjectDescriptor d = obj.describe();

  for (const auto &[key, value] : d.fields) {
    out[key] = makeObjStrRuntimeValue(value);
  }

  return {DataType::Object, std::move(out)};
}

} // namespace TerminalCommands