#pragma once
#include "GameCamera.hpp"
#include "GameText.hpp"

class Terminal {
public:
  Terminal(sf::RenderWindow &window);
  ~Terminal();

  void handleEvent(const sf::Event &event);
  void update();

  [[nodiscard]] sf::RenderWindow *getTargetWindow() const;

  void close();
  [[nodiscard]] bool isOpen() const;

private:
  void rebuildText();
  void executeCommand(const std::string &command);

private:
  sf::RenderWindow &targetWindow;
  GameCamera *camera;

  GameText *text;

  std::deque<std::string> history;
  std::string input;

  bool opened = true;
};