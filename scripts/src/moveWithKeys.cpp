#include "moveWithKeys.hpp"
#include "GameObject.hpp"
#include "GameState.hpp"
#include "InputManager.hpp"

namespace script {

namespace {

const float SPEED = 3.f * 50.f;

} // namespace

void moveWithKeys(TangibleObject &obj, const GeneralContext &ctx) {

  GameState &gameState = GameState::getInstance();

  if (InputManager::getInstance().isPressed("right")) {
    obj.position.x += SPEED * gameState.dt();
  } else if (InputManager::getInstance().isPressed("left")) {
    obj.position.x -= SPEED * gameState.dt();
  }

  if (InputManager::getInstance().isPressed("up")) {
    obj.position.y -= SPEED * gameState.dt();
  } else if (InputManager::getInstance().isPressed("down")) {
    obj.position.y += SPEED * gameState.dt();
  }
}

} // namespace script