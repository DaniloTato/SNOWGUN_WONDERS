#include "terminalCreationScript.hpp"

#include "GameState.hpp"
#include "InputManager.hpp"
#include "Terminal.hpp"

namespace script {

namespace {
struct terminalCreationState {
  Terminal *terminal = nullptr;
};
} // namespace

void terminalCreationScript(ScriptRunner &renderable,
                            const GeneralContext &ctx) {

  auto &state =
      renderable.scripter.getState<terminalCreationState>("terminalCreation");

  if (InputManager::getInstance().isJustPressed("terminal")) {
    if (state.terminal) {
      state.terminal->close();
      delete state.terminal;
      state.terminal = nullptr;
      GameState::getInstance().removeWindow(GameState::WindowTypes::TERMINAL);
    } else {
      GameState::getInstance().createWindow(GameState::WindowTypes::TERMINAL,
                                            900, 400, "Snowgun Terminal");
      state.terminal =
          new Terminal(*GameState::getInstance().getTerminalWindow());
    }
  }

  if (state.terminal) {
    sf::Event event;
    while (state.terminal->getTargetWindow()->pollEvent(event)) {
      state.terminal->handleEvent(event);
    }
    state.terminal->update();
  }
}

} // namespace script