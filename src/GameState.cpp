#include "GameState.hpp"
#include "Bullet.hpp"
#include "Constants.hpp"
#include "GameCamera.hpp"
#include "SFML/System/Vector2.hpp"
#include "Terminal.hpp"
#include <SFML/Graphics.hpp>

GameState::GameState()
    : activeCameras({}),
      activeWindows({new sf::RenderWindow(
          sf::VideoMode(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT),
          Constants::MAIN_WINDOW_NAME)}),
      checkpoint({-1, -1}), playerHealth(3),
      selectedWeapon(Bullet::Type::Normal) {}

GameState &GameState::getInstance() {
  static GameState instance;
  return instance;
}

void GameState::removeWindow(WindowTypes type) {
  auto index = static_cast<size_t>(type);
  if (activeWindows.size() > index && activeWindows[index]) {
    if (activeWindows[index]->isOpen()) {
      activeWindows[index]->close();
    }

    Renderizer::unregisterByWindow(activeWindows[index]);
    delete activeWindows[index];
    activeWindows[index] = nullptr;
  }
}

void GameState::removeWindow(sf::RenderWindow *reference) {
  for (size_t i = 0; i < activeWindows.size(); ++i) {
    if (activeWindows[i] == reference) {
      removeWindow(static_cast<WindowTypes>(i));
      return;
    }
  }
}

const std::vector<GameCamera *> &GameState::getActiveCameras() const {
  return activeCameras;
}

GameCamera *GameState::getMainCamera() const {
  size_t mainCameraIndex = static_cast<int>(CameraTypes::MAIN);
  if (activeCameras.size() >= mainCameraIndex) {
    return activeCameras[mainCameraIndex];
  }
  return nullptr;
}

sf::RenderWindow *GameState::getMainWindow() const {
  if (!activeWindows.empty()) {
    return activeWindows.front();
  }
  return nullptr;
}

sf::RenderWindow *GameState::getTerminalWindow() const {
  size_t terminalWindowIndex = static_cast<int>(WindowTypes::TERMINAL);
  if (activeWindows.size() >= terminalWindowIndex) {
    return activeWindows[terminalWindowIndex];
  }
  return nullptr;
}

void GameState::clearCameras() {
  for (auto &i : activeCameras) {
    GameObject::destroy(i);
  }
  activeCameras.clear();
}

float GameState::dt() { return dtValue; }

void GameState::updateDt() {
  auto now = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> elapsed = now - lastFrameTime;
  lastFrameTime = now;
  dtValue = elapsed.count();
}

GameCamera *GameState::getUiCamera() const {
  size_t uiCameraIndex = static_cast<int>(CameraTypes::UI);
  if (activeCameras.size() >= uiCameraIndex) {
    return activeCameras[uiCameraIndex];
  }
  return nullptr;
}

GameCamera *GameState::getTerminalCamera() const {
  size_t consoleCameraIndex = static_cast<int>(CameraTypes::TERMINAL);
  if (activeCameras.size() >= consoleCameraIndex) {
    return activeCameras[consoleCameraIndex];
  }
  return nullptr;
}

void GameState::createCamera(CameraTypes type) {

  auto index = static_cast<size_t>(type);

  if (activeCameras.size() < static_cast<int>(CameraTypes::COUNT)) {
    activeCameras.resize(static_cast<int>(CameraTypes::COUNT));
  }

  if (activeCameras[index]) {
    return;
  }

  activeCameras[index] = new GameCamera();
}

void GameState::createWindow(WindowTypes type, int width, int height,
                             const std::string &name) {

  auto index = static_cast<size_t>(type);

  if (activeWindows.size() < static_cast<int>(WindowTypes::COUNT)) {
    activeWindows.resize(static_cast<int>(WindowTypes::COUNT));
  }

  if (activeWindows[index]) {
    return;
  }

  activeWindows[index] =
      new sf::RenderWindow(sf::VideoMode(width, height), name);
}

int GameState::getCrystalAmount() const { return crystals; }

void GameState::addToCrystalAmount(int amount) { crystals += amount; }

int GameState::getPlayerHealth() const { return playerHealth; }

void GameState::changePlayerHealth(int amount) { playerHealth += amount; }

void GameState::changeWeaponSelection() {
  if (selectedWeapon == Bullet::Type::Normal) {
    selectedWeapon = Bullet::Type::BubbleGun;
  } else {
    selectedWeapon = Bullet::Type::Normal;
  }
}

Bullet::Type GameState::getWeaponSelection() const { return selectedWeapon; }

void GameState::setCheckpoint(sf::Vector2f position) { checkpoint = position; }

const sf::Vector2f GameState::getCheckpoint() const { return checkpoint; }

bool GameState::hasCheckpoint() const {
  return checkpoint != sf::Vector2f{-1, -1};
}

const std::vector<sf::RenderWindow *> &GameState::getWindows() const {
  return activeWindows;
}

sf::RenderWindow *GameState::getReferenceByType(WindowTypes type) {
  auto index = static_cast<size_t>(type);
  if (activeWindows.size() > index) {
    return activeWindows[index];
  } else {
    return nullptr;
  }
}

void GameState::updateGeneralContext(GeneralContext &ctx) {
  generalContext = ctx;

  Terminal::memory.declareVariable(
      "player",
      TerminalCommands::RuntimeValue{
          TerminalCommands::DataType::GameObjectReference, ctx.player});
}

const GeneralContext &GameState::getGeneralContext() { return generalContext; }