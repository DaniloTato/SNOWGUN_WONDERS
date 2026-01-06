#include "updateCrystalCounterScript.hpp"

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

struct CrystalCounterState {
  GameText *crystalCounter = nullptr;
  bool setup = false;
};
} // namespace

void updateCrystalCounterScript(ScriptRunner &runner,
                                const GeneralContext &ctx) {

  auto &state = runner.scripter.getState<CrystalCounterState>(
      "updateCrystalCounterScript");

  if (!state.setup) {
    auto &window = *GameState::getInstance().getMainWindow();
    auto *camera = GameState::getInstance().getUiCamera();

    RenderizerParameters params{
        window,     fontTexture, sf::IntRect(),
        {0.f, 0.f}, camera,      Constants::UI_TEXT_LAYER,
        1.f};

    state.crystalCounter = new GameText(params);

    state.setup = true;
  }

  std::string markupCrystal =
      "#position 0 12\n"
      "#boundary " +
      std::to_string(Constants::SCREEN_WIDTH / 3 - 16) +
      "\n"
      "#alignment right\n" +
      std::to_string(GameState::getInstance().getCrystalAmount());

  state.crystalCounter->loadFromMarkup(markupCrystal);
}
} // namespace script