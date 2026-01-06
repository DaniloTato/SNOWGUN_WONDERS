#include "Terminal.hpp"

#include "Constants.hpp"
#include "GameCamera.hpp"
#include "GameState.hpp"
#include "GameText.hpp"
#include "Helpers.hpp"
#include "TerminalExpr.hpp"
#include "TerminalParser.hpp"

#include <SFML/Graphics.hpp>
#include <string_view>

static constexpr size_t MAX_LINES = 25;

std::unordered_map<std::string, TerminalCommands::Entry> Terminal::commandMap;
std::vector<Terminal *> Terminal::s_activeTerminals;

std::unordered_map<std::string, TerminalCommands::Entry> &
Terminal::getCommandMap() {
  return commandMap;
}

Terminal::Terminal(sf::RenderWindow *window, GameCamera *camera)
    : targetWindow(window),
      text({*targetWindow,
            Helper::loadTexture(Helper::getPath("assets/font.png")),
            sf::IntRect(),
            {10.f, 10.f},
            camera,
            Constants::UI_LAYER,
            1.f}) {

  s_activeTerminals.push_back(this);

  history.emplace_back("<color=cyan>Snowgun Developer Console</color>");
  history.emplace_back("Type <color=yellow>help</color> for commands");

  rebuildText();
}

void Terminal::handleEvent(const sf::Event &event) {

  if (event.type == sf::Event::TextEntered) {
    if (event.text.unicode >= 32 && event.text.unicode < 127) {
      input += static_cast<char>(event.text.unicode);
      rebuildText();
    }
  }

  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Escape) {
      kill();
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

void Terminal::kill() { opened = false; }

void Terminal::close() {
  kill();
  destroyWindowOnClose = true;
}

void Terminal::print(std::string_view message, std::string_view color) {
  std::string out;
  if (!color.empty()) {
    out = "<color=";
    out += color;
    out += ">";
    out += message;
    out += "</color>";
  } else {
    out = message;
  }

  history.emplace_back(out);
}

void Terminal::lineJump() { history.emplace_back("<ln>"); }

bool Terminal::isOpen() const { return opened; }

void Terminal::rebuildText() {
  std::string markup;
  markup += "#position 10 10\n";
  markup += "#boundary 880\n";

  for (const auto &line : history) {
    markup += line + "\n";
  }

  markup += "<color=yellow>> " + input + "</color>";

  text.loadFromMarkup(markup);
}

void Terminal::executeCommand(const std::string &input) {
  try {
    TerminalParser parser(input);

    std::shared_ptr<Expr> expr = parser.parse();
    if (!expr) {
      print("Parse error", "red");
      return;
    }

    TerminalCommands::RuntimeValue value = expr->eval(*this);

    std::string out = interpreter.toString(value);
    print(out);
    lineJump();

  } catch (const std::exception &e) {
    print(e.what(), "red");
  }
}

sf::RenderWindow *Terminal::getTargetWindow() const { return targetWindow; }

bool Terminal::destroysWindowOnClose() const { return destroyWindowOnClose; }

void Terminal::clear() { history.clear(); }

void Terminal::destroyKilledTerminals() {
  s_activeTerminals.erase(
      std::remove_if(s_activeTerminals.begin(), s_activeTerminals.end(),
                     [](Terminal *t) {
                       if (!t->isOpen()) {
                         if (t->destroysWindowOnClose()) {
                           GameState::getInstance().removeWindow(
                               t->getTargetWindow());
                         }
                         delete t;
                         return true;
                       }
                       return false;
                     }),
      s_activeTerminals.end());
}

void Terminal::registerCommand(const TerminalCommands::Entry &cmd) {
  commandMap.emplace(cmd.invocation, cmd);
}

TerminalMemory Terminal::memory;

TerminalMemory &Terminal::getMemory() { return memory; }

TerminalInterpreter &Terminal::getInterpreter() { return interpreter; }