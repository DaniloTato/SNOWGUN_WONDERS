#pragma once

#include "Bullet.hpp"
#include "GameCamera.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"
#include <chrono>
#include <vector>

class GameState {
public:
  enum class CameraList : std::uint8_t { MAIN, UI, COUNT };

  static GameState &getInstance();

  void addCamera(GameCamera *camera);
  void removeCamera(GameCamera *camera);
  void createCamera(CameraList type);
  [[nodiscard]] const std::vector<GameCamera *> &getActiveCameras() const;
  [[nodiscard]] GameCamera *getMainCamera() const;
  [[nodiscard]] GameCamera *getUiCamera() const;
  [[nodiscard]] sf::RenderWindow *getMainWindow() const;
  void clearCameras();
  float dt();
  void updateDt();

  void setCheckpoint(sf::Vector2f position);
  [[nodiscard]] const sf::Vector2f getCheckpoint() const;

  [[nodiscard]] int getCrystalAmount() const;
  void addToCrystalAmount(int amount);

  [[nodiscard]] int getPlayerHealth() const;
  void changePlayerHealth(int amount);

  void changeWeaponSelection();
  [[nodiscard]] Bullet::Type getWeaponSelection() const;
  [[nodiscard]] bool hasCheckpoint() const;

  GameState(const GameState &) = delete;
  GameState &operator=(const GameState &) = delete;

  GameState(GameState &&) = delete;
  GameState &operator=(GameState &&) = delete;

private:
  GameState();
  ~GameState() = default;

  std::vector<GameCamera *> activeCameras;
  std::vector<sf::RenderWindow *> activeWindows;

  std::chrono::high_resolution_clock::time_point lastFrameTime =
      std::chrono::high_resolution_clock::now();
  float dtValue;

  sf::Vector2f checkpoint;

  int crystals;
  int playerHealth;
  Bullet::Type selectedWeapon;
};