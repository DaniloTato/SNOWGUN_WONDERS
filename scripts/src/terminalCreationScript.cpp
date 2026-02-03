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
    if (!state.terminal) {
      GameState::getInstance().createCamera(CameraTypes::TERMINAL);
      GameState::getInstance().createWindow(WindowTypes::TERMINAL, 900, 400,
                                            "Snowgun Terminal");
      state.terminal =
          new Terminal(GameState::getInstance().getTerminalWindow(),
                       GameState::getInstance().getTerminalCamera());
    }
  }

  if (state.terminal) {
    sf::Event event;
    while (state.terminal->getTargetWindow()->pollEvent(event)) {

      if (event.type == sf::Event::Closed) {
        if (state.terminal->isOpen()) {
          state.terminal->close();
          continue;
        }
      }

      state.terminal->handleEvent(event);
    }

    state.terminal->update();

    if (!state.terminal->isOpen()) {
      state.terminal = nullptr;
    }
  }
}

} // namespace script