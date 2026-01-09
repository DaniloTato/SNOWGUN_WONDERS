#pragma once
#include "GameCamera.hpp"
#include "GameText.hpp"
#include "Highlighter.hpp"
#include "TerminalCommands.hpp"
#include "TerminalError.hpp"
#include "TerminalInterpreter.hpp"
#include "TerminalMemory.hpp"

class Terminal {
public:
  Terminal(sf::RenderWindow *window, GameCamera *camera);
  ~Terminal();

  void handleEvent(const sf::Event &event);

  [[nodiscard]] sf::RenderWindow *getTargetWindow() const;

  void kill();
  void close();
  void clear();
  [[nodiscard]] bool isOpen() const;

  static void destroyKilledTerminals();

  [[nodiscard]] bool destroysWindowOnClose() const;

  static void registerCommand(const TerminalCommands::CommandEntry &cmd);

  void print(std::string_view message, std::string_view color = {});

  static std::unordered_map<std::string, TerminalCommands::CommandEntry> &
  getCommandMap();

  void lineJump();

  TerminalError error;
  TerminalInterpreter interpreter;
  static TerminalMemory memory;
  Highlighter highlighter;

private:
  void rebuildText();
  void executeCommand(const std::string &command);

private:
  sf::RenderWindow *targetWindow;

  std::unordered_map<std::string, std::string> aliases;

  GameText *text;

  std::deque<std::string> history;
  std::vector<std::string> inputHistory;
  std::string input;

  static std::unordered_map<std::string, TerminalCommands::CommandEntry>
      commandMap;
  static std::vector<Terminal *> s_activeTerminals;

  bool opened = true;
  bool destroyWindowOnClose = false;

  size_t cursorPos = 0;

  int historyBrowseIndex = -1;
  std::string savedInput;
};