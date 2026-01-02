#include "Terminal.hpp"

#include "Constants.hpp"
#include "GameCamera.hpp"
#include "GameState.hpp"
#include "GameText.hpp"
#include "Helpers.hpp"
#include "Renderizer.hpp"

#include <SFML/Graphics.hpp>

static constexpr size_t MAX_LINES = 25;

Terminal::Terminal(sf::RenderWindow &window) : targetWindow(window) {
  GameState &gameState = GameState::getInstance();
  gameState.createCamera(GameState::CameraTypes::TERMINAL);
  camera = gameState.getTerminalCamera();

  sf::Texture &fontTexture =
      Helper::loadTexture(Helper::getPath("assets/font.png"));

  RenderizerParameters params{targetWindow, fontTexture, sf::IntRect(),
                              {10.f, 10.f}, camera,      Constants::UI_LAYER,
                              1.f};

  text = new GameText(params);
  text->setFontAtlas(fontTexture, 9, 8, 95, 32);

  history.emplace_back("<color=cyan>Snowgun Developer Console</color>");
  history.emplace_back("Type <color=yellow>help</color> for commands");

  rebuildText();
}

Terminal::~Terminal() { delete text; }

void Terminal::handleEvent(const sf::Event &event) {
  if (event.type == sf::Event::Closed) {
    close();
    return;
  }

  if (event.type == sf::Event::TextEntered) {
    if (event.text.unicode >= 32 && event.text.unicode < 127) {
      input += static_cast<char>(event.text.unicode);
      rebuildText();
    }
  }

  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Escape) {
      close();
    }

    if (event.key.code == sf::Keyboard::Backspace && !input.empty()) {
      input.pop_back();
      rebuildText();
    }

    if (event.key.code == sf::Keyboard::Enter) {
      history.push_back("> " + input);
      executeCommand(input);
      input.clear();

      while (history.size() > MAX_LINES) {
        history.pop_front();
      }

      rebuildText();
    }
  }
}

void Terminal::update() {
  camera->goTo({0.f, 0.f});
  camera->goToDesired();
  camera->zoomTo(1.0f);
  camera->zoomToDesired();
}

void Terminal::close() {
  opened = false;
  targetWindow.close();
}

bool Terminal::isOpen() const { return opened; }

void Terminal::rebuildText() {
  std::string markup;
  markup += "#position 10 10\n";
  markup += "#boundary 880\n";

  for (const auto &line : history) {
    markup += line + "\n";
  }

  markup += "<color=yellow>> " + input + "</color>";

  text->loadFromMarkup(markup);
}

void Terminal::executeCommand(const std::string &command) {
  if (command == "help") {
    history.emplace_back("<color=green>Available commands:</color>");
    history.emplace_back(
        "  <color=yellow>help</color>        - Show this message");
    history.emplace_back("  <color=yellow>clear</color>       - Clear console");
    history.emplace_back("  <color=yellow>exit</color>        - Close console");
    return;
  }

  if (command == "clear") {
    history.clear();
    return;
  }

  if (command == "exit") {
    close();
    return;
  }

  history.emplace_back("<color=red>Unknown command:</color> " + command);
}

sf::RenderWindow *Terminal::getTargetWindow() const { return &targetWindow; }