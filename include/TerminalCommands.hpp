#pragma once

#include "GameState.hpp"
#include <string>
#include <vector>

class Terminal;
class GameObject;
struct Expr;
struct Variable;
struct Function;

namespace TerminalCommands {

struct Context {
  GameState &gameState = GameState::getInstance();
  Terminal *terminal = nullptr;
  std::unordered_map<std::string, std::string> aliases =
      std::unordered_map<std::string, std::string>();
};

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
  Expression
};

struct RuntimeValue {
  DataType type = DataType::Null;

  using Parameter = std::string;
  using Object = std::unordered_map<std::string, RuntimeValue>;
  using List = std::vector<RuntimeValue>;
  using GameObjectReference = GameObject *;
  using RuntimeVariable = Variable *;
  using Number = float;

  using FunctionPtr = Function *;
  using Expression = std::shared_ptr<Expr>;

  std::variant<std::monostate, GameObjectReference, RuntimeVariable, Object,
               Parameter, DataType, Number, List, FunctionPtr, Expression>
      data = std::monostate();
};

using ArgsVector = std::vector<RuntimeValue>;
using CommandFn = RuntimeValue (*)(Terminal *, Context &, const ArgsVector &);

struct Entry {
  std::string invocation;
  CommandFn execute = nullptr;
  std::string description = "no description";
};

void registerAll();

RuntimeValue makeObjStrRuntimeValue(const GameObjectDescriptor::Value &value);

RuntimeValue makeObjectView(const GameObject &obj);

} // namespace TerminalCommands
