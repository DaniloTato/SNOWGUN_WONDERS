#pragma once

#include "TerminalCommands.hpp"
#include <vector>

class TerminalError {
public:
  [[noreturn]] void cmdError(const std::string &msg) {
    throw std::runtime_error(msg);
  }

  void validate(const TerminalCommands::CommandSignature &sig,
                const TerminalCommands::ArgsVector &args) {

    if (sig.variadic)
      return;

    if (args.size() < sig.minArgs || args.size() > sig.maxArgs) {
      cmdError("[" + sig.name + "] expected between " +
               std::to_string(sig.minArgs) + " and " +
               std::to_string(sig.maxArgs) + " arguments");
    }

    for (size_t i = 0; i < sig.positionalTypes.size() && i < args.size(); ++i) {

      if (sig.positionalTypes[i] == TerminalCommands::DataType::Any) {
        continue;
      }

      if (args[i].type != sig.positionalTypes[i]) {
        cmdError("[" + sig.name + "] argument " + std::to_string(i + 1) +
                 " has wrong type");
      }
    }
  }
};