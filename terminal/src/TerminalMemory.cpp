#include "TerminalMemory.hpp"

TerminalMemory::TerminalMemory() { scopes.emplace_back(); }

bool TerminalMemory::declareVariable(std::string_view name,
                                     TerminalCommands::RuntimeValue value,
                                     bool isConst) {

  RuntimeScope &scope = scopes.back();

  auto [it, inserted] =
      scope.try_emplace(std::string(name), Variable{.value = std::move(value),
                                                    .isConst = isConst});
  return inserted;
}

bool TerminalMemory::setVariable(std::string_view name,
                                 TerminalCommands::RuntimeValue value) {
  for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
    auto found = it->find(std::string(name));
    if (found == it->end())
      continue;

    if (found->second.isConst)
      return false;

    found->second.value = std::move(value);
    return true;
  }

  return false;
}

Variable *TerminalMemory::getVariable(std::string_view name) {
  for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
    auto found = it->find(std::string(name));
    if (found != it->end())
      return &found->second;
  }
  return nullptr;
}

void TerminalMemory::pushScope() { scopes.emplace_back(); }
void TerminalMemory::popScope() { scopes.pop_back(); }