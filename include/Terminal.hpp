#pragma once
#include "GameCamera.hpp"
#include "GameText.hpp"
#include "TerminalCommands.hpp"
#include "TerminalInterpreter.hpp"
#include "TerminalMemory.hpp"

class Terminal {
public:
  Terminal(sf::RenderWindow *window, GameCamera *camera);
  ~Terminal() = default;

  void handleEvent(const sf::Event &event);

  [[nodiscard]] sf::RenderWindow *getTargetWindow() const;

  void kill();
  void close();
  void clear();
  [[nodiscard]] bool isOpen() const;

  static void destroyKilledTerminals();

  bool destroysWindowOnClose() const;

  static void registerCommand(const TerminalCommands::Entry &cmd);

  void print(std::string_view message, std::string_view color = {});

  static std::unordered_map<std::string, TerminalCommands::Entry> &
  getCommandMap();

  void lineJump();

  static TerminalMemory &getMemory();
  TerminalInterpreter &getInterpreter();

private:
  void rebuildText();
  void executeCommand(const std::string &command);

private:
  static TerminalMemory memory;
  TerminalInterpreter interpreter;

  sf::RenderWindow *targetWindow;

  std::unordered_map<std::string, std::string> aliases;

  GameText text;

  std::deque<std::string> history;
  std::string input;

  static std::unordered_map<std::string, TerminalCommands::Entry> commandMap;
  static std::vector<Terminal *> s_activeTerminals;

  bool opened = true;
  bool destroyWindowOnClose = false;
};