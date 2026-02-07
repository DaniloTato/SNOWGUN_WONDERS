#pragma once

#include "Bullet.hpp"
#include "GameCamera.hpp"
#include "GameObjectExposure.hpp"
#include "GeneralContext.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"
#include <chrono>
#include <vector>

enum class CameraTypes : std::uint8_t { MAIN, UI, TERMINAL, COUNT };
enum class WindowTypes : std::uint8_t { MAIN, TERMINAL, COUNT };

struct WindowCreationRequest {
  WindowTypes type;
  int width;
  int height;
  std::string name;
};

struct GameWindow {
  sf::RenderWindow *window;
  int frameRate;
};

class GameState {
public:
  static GameState &getInstance();

  void createCamera(CameraTypes type);

  void removeWindow(WindowTypes type);
  void removeWindow(sf::RenderWindow *reference);
  void createWindow(WindowTypes type, int width, int height,
                    const std::string &name);

  sf::RenderWindow *getReferenceByType(WindowTypes type);

  [[nodiscard]] const std::vector<sf::RenderWindow *> &getWindows() const;

  [[nodiscard]] const std::vector<GameCamera *> &getActiveCameras() const;
  [[nodiscard]] GameCamera *getMainCamera() const;
  [[nodiscard]] GameCamera *getUiCamera() const;
  [[nodiscard]] GameCamera *getTerminalCamera() const;
  [[nodiscard]] sf::RenderWindow *getMainWindow() const;
  [[nodiscard]] sf::RenderWindow *getTerminalWindow() const;
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

  void updateGeneralContext(GeneralContext &ctx);
  [[nodiscard]] const GeneralContext &getGeneralContext();

  [[nodiscard]] const GameObjectExposure::Descriptor &
  getExposedGameState() const;

  void setPrintingObjectIds(bool value);
  [[nodiscard]] bool getPrintingObjectIds() const;

  void setFrameRate(WindowTypes type, int value);
  [[nodiscard]] int getFrameRate(WindowTypes type) const;

  GameState(const GameState &) = delete;
  GameState &operator=(const GameState &) = delete;

  GameState(GameState &&) = delete;
  GameState &operator=(GameState &&) = delete;

private:
  GameState();

  std::vector<GameCamera *> activeCameras;
  std::vector<sf::RenderWindow *> activeWindows;

  std::chrono::high_resolution_clock::time_point lastFrameTime =
      std::chrono::high_resolution_clock::now();
  float dtValue;

  GeneralContext generalContext;

  GameObjectExposure::Descriptor exposedGameState;

  sf::Vector2f checkpoint;

  int crystals;
  int playerHealth;
  Bullet::Type selectedWeapon;

  bool printingGameObjectIds = false;
};