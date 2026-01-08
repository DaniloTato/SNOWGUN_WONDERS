#pragma once

#include <string>
#include <unordered_map>
#include <vector>

class Terminal;
class GameObject;
struct Expr;
struct Variable;
struct Function;

namespace TerminalCommands {

enum class DataType : std::uint8_t {
  Null,
  GameObjectReference,
  Object,
  Variable,
  Parameter,
  Type,
  Message,
  Number,
  List,
  Function,
  Expression,
  Bool,
  Any
};

struct RuntimeValue {
  DataType type = DataType::Null;

  using Parameter = std::string;
  using Object = std::unordered_map<std::string, RuntimeValue>;
  using List = std::vector<RuntimeValue>;
  using GameObjectReference = GameObject *;
  using RuntimeVariable = Variable *;
  using Number = float;
  using Bool = bool;

  using FunctionPtr = Function *;
  using Expression = std::shared_ptr<Expr>;

  std::variant<std::monostate, GameObjectReference, RuntimeVariable, Object,
               Parameter, DataType, Number, List, FunctionPtr, Expression, Bool>
      data = std::monostate();
};

using ArgsVector = std::vector<RuntimeValue>;

struct CommandSignature {
  std::string name;
  std::string description;
  size_t minArgs = 0;
  size_t maxArgs = 0;
  std::vector<DataType> positionalTypes = {};
  bool variadic = false;
};

using CommandFn = RuntimeValue (*)(Terminal *, CommandSignature &,
                                   const ArgsVector &);

struct CommandEntry {
  CommandFn function;
  CommandSignature signature;
  CommandEntry(CommandFn function, CommandSignature signature)
      : function(function), signature(std::move(signature)) {}
};

void registerAll();

} // namespace TerminalCommands
