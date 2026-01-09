#include "TerminalCommands.hpp"
#include "Terminal.hpp"
#include "TerminalExpr.hpp"

#include <string>

namespace TerminalCommands {

namespace {

TerminalCommands::RuntimeValue echoCommand(Terminal *owner,
                                           CommandSignature &signature,
                                           const ArgsVector &args) {

  owner->error.validate(signature, args);
  std::string out;

  for (size_t i = 0; i < args.size(); ++i) {
    if (i)
      out += " ";

    out += owner->interpreter.toString(args[i]);
  }

  return {.type = DataType::Parameter, .data = out};
}

TerminalCommands::RuntimeValue helpCommand(Terminal *owner,
                                           CommandSignature &signature,
                                           const ArgsVector &args) {

  owner->error.validate(signature, args);

  std::string out;
  out += "<color=green>Available commands:</color>\n";

  for (const auto &[name, cmd] : Terminal::getCommandMap()) {
    out += "  \\<color=yellow\\>" + name +
           "\\</color\\>: " + cmd.signature.description + "\n";
  }

  return {.type = DataType::Message, .data = out};
}

TerminalCommands::RuntimeValue killCommand(Terminal *owner,
                                           CommandSignature &signature,
                                           const ArgsVector &args) {
  owner->error.validate(signature, args);
  owner->kill();
  return {};
}

TerminalCommands::RuntimeValue exitCommand(Terminal *owner,
                                           CommandSignature &signature,
                                           const ArgsVector &args) {
  owner->error.validate(signature, args);
  owner->close();
  return {};
}

RuntimeValue getCommand(Terminal *owner, CommandSignature &signature,
                        const ArgsVector &args) {

  owner->error.validate(signature, args);

  const RuntimeValue &name = args[0];

  const auto &str = std::get<std::string>(name.data);
  Variable *ref = owner->memory.getVariable(str);

  if (!ref) {
    owner->error.cmdError("[GET] unknown reference: " + str);
  }

  return {.type = DataType::Variable, .data = ref};
}

TerminalCommands::RuntimeValue newCommand(Terminal *owner,
                                          CommandSignature &signature,
                                          const ArgsVector &args) {

  owner->error.validate(signature, args);

  const RuntimeValue &name = args[0];

  auto &str = std::get<std::string>(name.data);

  bool success = owner->memory.declareVariable(
      str, RuntimeValue{.type = DataType::Null, .data = std::monostate()});

  if (!success) {
    owner->error.cmdError("[NEW] variable was previously declared");
  }

  return {.type = DataType::Variable, .data = owner->memory.getVariable(str)};
}

TerminalCommands::RuntimeValue typeCommand(Terminal *owner,
                                           CommandSignature &signature,
                                           const ArgsVector &args) {

  owner->error.validate(signature, args);

  const RuntimeValue &strType = args[0];

  DataType type =
      owner->interpreter.stringToDataType(std::get<std::string>(strType.data));

  if (type == DataType::Null) {
    owner->error.cmdError("[TYPE] invalid data type.");
  }

  return {.type = DataType::Type, .data = type};
}

RuntimeValue numCommand(Terminal *owner, CommandSignature &signature,
                        const ArgsVector &args) {

  owner->error.validate(signature, args);

  const RuntimeValue &strNum = args[0];

  const auto &param = std::get<RuntimeValue::Parameter>(strNum.data);

  try {
    return {.type = DataType::Number, .data = std::stof(param)};
  } catch (...) {
    owner->error.cmdError("[VAL] invalid number");
  }
}

TerminalCommands::RuntimeValue listCommand(Terminal *owner,
                                           CommandSignature &signature,
                                           const ArgsVector &args) {
  TerminalCommands::RuntimeValue::List list;
  list.reserve(args.size());

  for (const auto &arg : args) {
    list.push_back(arg);
  }

  return {.type = DataType::List, .data = std::move(list)};
}

TerminalCommands::RuntimeValue clearCommand(Terminal *owner,
                                            CommandSignature &signature,
                                            const ArgsVector &args) {
  owner->clear();
  return {.type = DataType::Message, .data = ""};
}

RuntimeValue funCommand(Terminal *owner, CommandSignature &signature,
                        const ArgsVector &args) {

  owner->error.validate(signature, args);

  const RuntimeValue &paramsExpr = args[0];
  const RuntimeValue &bodyExpr = args[1];

  auto params = std::get<RuntimeValue::List>(paramsExpr.data);

  for (const auto &val : params) {
    if (val.type != DataType::Parameter) {
      owner->error.cmdError(
          "[fun] 1st argument list must only contain raw parameters");
    }
  }

  auto *fn = new Function; // watch out! right now, if not assigned to a
                           // variable, functions are dangling ptrs
  fn->parameters = params;
  fn->body = std::get<RuntimeValue::Expression>(bodyExpr.data);
  fn->closure = &owner->memory;

  return {.type = DataType::Function, .data = fn};
}

TerminalCommands::RuntimeValue doCommand(Terminal *owner,
                                         CommandSignature &signature,
                                         const ArgsVector &args) {

  if (args.empty()) {
    throw std::runtime_error(
        "[do] must specify the function or expression to execute");
  }

  const RuntimeValue &fnVal = args[0];

  if (fnVal.type == DataType::Expression) {

    if (args.size() > 1) {
      throw std::runtime_error("[do] expressions receive no arguments");
    }

    auto expr =
        std::get<TerminalCommands::RuntimeValue::Expression>(fnVal.data);

    return expr->eval(*owner);
  }

  if (fnVal.type != DataType::Function) {
    throw std::runtime_error(
        "[do] first argument is not a function or expression");
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

TerminalCommands::RuntimeValue ifCommand(Terminal *owner,
                                         CommandSignature &signature,
                                         const ArgsVector &args) {

  owner->error.validate(signature, args);

  const RuntimeValue &boolSatement = args[0];
  const RuntimeValue &bodyExpr = args[1];
  RuntimeValue elseBodyExpr = {};
  if (args.size() >= 3) {
    elseBodyExpr = args[2];
  }

  if (std::get<TerminalCommands::RuntimeValue::Bool>(boolSatement.data)) {
    return bodyExpr;
  } else {
    if (elseBodyExpr.type == DataType::Null)
      return {};
    return elseBodyExpr;
  }

  return {};
}

TerminalCommands::RuntimeValue doifCommand(Terminal *owner,
                                           CommandSignature &signature,
                                           const ArgsVector &args) {

  owner->error.validate(signature, args);

  const RuntimeValue &boolSatement = args[0];
  const RuntimeValue &bodyExpr = args[1];
  std::shared_ptr<Expr> elseBody;
  if (args.size() >= 3) {
    elseBody =
        std::get<TerminalCommands::RuntimeValue::Expression>(args[2].data);
  }

  auto ifBody =
      std::get<TerminalCommands::RuntimeValue::Expression>(bodyExpr.data);

  if (std::get<TerminalCommands::RuntimeValue::Bool>(boolSatement.data)) {
    return ifBody->eval(*owner);
  } else {
    if (elseBody)
      return elseBody->eval(*owner);
    return {};
  }

  return {};
}

} // namespace

void registerAll() {

  CommandSignature helpSignature = {.name = "help",
                                    .description =
                                        "get list of all registered commmands",
                                    .minArgs = 0,
                                    .maxArgs = 0};
  Terminal::registerCommand({helpCommand, helpSignature});

  CommandSignature killSignature = {
      .name = "kill",
      .description = "destroy console instance (does not close window)",
      .minArgs = 0,
      .maxArgs = 0};
  Terminal::registerCommand({killCommand, killSignature});

  CommandSignature exitSignature = {
      .name = "exit",
      .description = "destroy console instance and close window",
      .minArgs = 0,
      .maxArgs = 0};
  Terminal::registerCommand({exitCommand, exitSignature});

  CommandSignature echoSignature = {.name = "echo",
                                    .description =
                                        "print the returned value of the input",
                                    .variadic = true};
  Terminal::registerCommand({echoCommand, echoSignature});

  CommandSignature getSignature = {
      .name = "get",
      .description = "returns a reference to a variable instance",
      .minArgs = 1,
      .maxArgs = 1,
      .positionalTypes = {DataType::Parameter}};
  Terminal::registerCommand({getCommand, getSignature});

  CommandSignature NewSignature = {.name = "new",
                                   .description = "declares a new variable",
                                   .minArgs = 1,
                                   .maxArgs = 1,
                                   .positionalTypes = {DataType::Parameter}};
  Terminal::registerCommand({newCommand, NewSignature});

  CommandSignature typeSignature = {.name = "type",
                                    .description =
                                        "converts parameter into data type",
                                    .minArgs = 1,
                                    .maxArgs = 1,
                                    .positionalTypes = {DataType::Parameter}};
  Terminal::registerCommand({typeCommand, typeSignature});

  CommandSignature numSignature = {.name = "num",
                                   .description =
                                       "converts parameter into number",
                                   .minArgs = 1,
                                   .maxArgs = 1,
                                   .positionalTypes = {DataType::Parameter}};
  Terminal::registerCommand({numCommand, numSignature});

  CommandSignature listSignature = {.name = "list",
                                    .description =
                                        "groups parameters into a list",
                                    .variadic = true};
  Terminal::registerCommand({listCommand, listSignature});

  CommandSignature clearSignature = {.name = "clear",
                                     .description =
                                         "clears the console history",
                                     .minArgs = 0,
                                     .maxArgs = 0};
  Terminal::registerCommand({clearCommand, clearSignature});

  CommandSignature funSignature = {
      .name = "fun",
      .description = "creates a function reference",
      .minArgs = 2,
      .maxArgs = 2,
      .positionalTypes = {DataType::List, DataType::Expression}};
  Terminal::registerCommand({funCommand, funSignature});

  CommandSignature doSignature = {.name = "do",
                                  .description =
                                      "executes a function or expression",
                                  .variadic = true};
  Terminal::registerCommand({doCommand, doSignature});

  CommandSignature ifSignature = {
      .name = "if",
      .description =
          "evaluates a bool statement and returns the corresponding expression",
      .minArgs = 2,
      .maxArgs = 3,
      .positionalTypes = {DataType::Bool, DataType::Any, DataType::Any}};
  Terminal::registerCommand({ifCommand, ifSignature});

  CommandSignature doifSignature = {.name = "doif",
                                    .description = "equivalent of do(if)",
                                    .minArgs = 2,
                                    .maxArgs = 3,
                                    .positionalTypes = {DataType::Bool,
                                                        DataType::Expression,
                                                        DataType::Expression}};
  Terminal::registerCommand({doifCommand, doifSignature});
}

} // namespace TerminalCommands