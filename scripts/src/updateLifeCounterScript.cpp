#include "updateLifeCounterScript.hpp"
#include "Constants.hpp"
#include "GameState.hpp"
#include "GameText.hpp"
#include "GeneralContext.hpp"
#include "Helpers.hpp"
#include "ScriptRunner.hpp"

namespace script {

namespace {

sf::Texture fontTexture =
    Helper::loadTexture((Helper::getPath("assets/font.png")));

struct LifeCounterState {
  GameText *lifeCounter = nullptr;
  bool setup = false;
};
} // namespace

void updateLifeCounterScript(ScriptRunner &runner, const GeneralContext &ctx) {

  auto &state = runner.scripter.getState<LifeCounterState>("updateLifeScript");

  if (!state.setup) {
    auto &window = *GameState::getInstance().getMainWindow();
    auto *camera = GameState::getInstance().getUiCamera();

    RenderizerParameters params{
        window,     fontTexture, sf::IntRect(),
        {0.f, 0.f}, camera,      Constants::UI_TEXT_LAYER,
        1.f};

    state.lifeCounter = new GameText(params);

    state.setup = true;
  }

  std::string hearts;
  for (int i = 0; i < GameState::getInstance().getPlayerHealth(); i++) {
    hearts += "*";
  }

  std::string markupLife = "#position 0 12\n"
                           "#boundary " +
                           std::to_string(Constants::SCREEN_WIDTH / 3) +
                           "\n"
                           "#alignment center\n" +
                           hearts;

  state.lifeCounter->loadFromMarkup(markupLife);
}
} // namespace script