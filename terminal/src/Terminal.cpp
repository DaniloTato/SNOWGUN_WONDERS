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

std::unordered_map<std::string, TerminalCommands::CommandEntry>
    Terminal::commandMap;
std::vector<Terminal *> Terminal::s_activeTerminals;

std::unordered_map<std::string, TerminalCommands::CommandEntry> &
Terminal::getCommandMap() {
  return commandMap;
}

Terminal::Terminal(sf::RenderWindow *window, GameCamera *camera)
    : targetWindow(window),
      text(
          new GameText({*targetWindow,
                        Helper::loadTexture(Helper::getPath("assets/font.png")),
                        sf::IntRect(),
                        {10.f, 10.f},
                        camera,
                        Constants::UI_LAYER,
                        1.f})) {

  s_activeTerminals.push_back(this);

  history.emplace_back(R"(\<color=cyan\>Snowgun Developer Console\</color\>)");
  history.emplace_back(R"(Type \<color=yellow\>help\</color\> for commands)");

  rebuildText();
}

Terminal::~Terminal() { GameObject::destroy(text); }

void Terminal::handleEvent(const sf::Event &event) {

  // TEXT INPUT (insert at cursor)
  if (event.type == sf::Event::TextEntered) {
    if (event.text.unicode >= 32 && event.text.unicode < 127 &&
        event.text.unicode != '\\') {

      char c = static_cast<char>(event.text.unicode);
      input.insert(input.begin() +
                       static_cast<std::string::difference_type>(cursorPos),
                   c);
      cursorPos++;

      rebuildText();
    }
  }

  // KEY INPUT
  if (event.type == sf::Event::KeyPressed) {

    // ESC
    if (event.key.code == sf::Keyboard::Escape) {
      kill();
      return;
    }

    // BACKSPACE
    if (event.key.code == sf::Keyboard::Backspace) {
      if (cursorPos > 0) {
        input.erase(input.begin() +
                    static_cast<std::string::difference_type>(cursorPos) - 1);
        cursorPos--;
        rebuildText();
      }
      return;
    }

    // LEFT
    if (event.key.code == sf::Keyboard::Left) {
      if (cursorPos > 0)
        cursorPos--;
      rebuildText();
      return;
    }

    // RIGHT
    if (event.key.code == sf::Keyboard::Right) {
      if (cursorPos < input.size())
        cursorPos++;
      rebuildText();
      return;
    }

    // UP (history back)
    if (event.key.code == sf::Keyboard::Up) {
      if (history.empty())
        return;

      if (historyBrowseIndex == -1) {
        savedInput = input;
        historyBrowseIndex = static_cast<int>(history.size()) - 1;
      } else if (historyBrowseIndex > 0) {
        historyBrowseIndex--;
      }

      input = history[historyBrowseIndex];
      cursorPos = input.size();
      rebuildText();
      return;
    }

    // DOWN (history forward)
    if (event.key.code == sf::Keyboard::Down) {
      if (historyBrowseIndex == -1)
        return;

      historyBrowseIndex++;

      if (historyBrowseIndex >= static_cast<int>(history.size())) {
        historyBrowseIndex = -1;
        input = savedInput;
      } else {
        input = history[historyBrowseIndex];
      }

      cursorPos = input.size();
      rebuildText();
      return;
    }

    // ENTER
    if (event.key.code == sf::Keyboard::Enter) {
      history.push_back(input);
      executeCommand(input);

      input.clear();
      cursorPos = 0;
      historyBrowseIndex = -1;

      while (history.size() > MAX_LINES)
        history.pop_front();

      rebuildText();
      return;
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
    out = "\\<color=";
    out += color;
    out += "\\>";
    out += message;
    out += "\\</color\\>";
  } else {
    out = message;
  }

  history.emplace_back(out);
}

void Terminal::lineJump() { history.emplace_back("\\<ln\\>"); }

bool Terminal::isOpen() const { return opened; }

void Terminal::rebuildText() {
  std::string markup;
  markup += "#position 10 10\n";
  markup += "#boundary 880\n";

  for (const auto &line : history) {
    markup += line + "\n";
  }

  std::string visibleInput = input;
  visibleInput.insert(cursorPos, "|");

  markup += "\\<color=yellow\\>> " + visibleInput + "\\</color\\>";

  if (text) {
    text->loadFromMarkup(markup);
  }
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

void Terminal::registerCommand(const TerminalCommands::CommandEntry &cmd) {
  commandMap.emplace(cmd.signature.name, cmd);
}

TerminalMemory Terminal::memory;