#pragma once
#include "Commands.hpp"
#include "RuntimeValue.hpp"
#include <functional>

namespace Snowlang {

class SnowlangInstance;
struct Executor;

using CommandFn = std::function<RuntimeValue(const Commands::CommandContext &ctx)>;

struct CommandSignature {
  std::string description;
  CommandFn function;

  CommandSignature(CommandFn function) : function(std::move(function)) {}
};

} // namespace Snowlang