#include "barrelScript.hpp"

#include "AnimatedObject.hpp"
#include "GameState.hpp"
#include "InputManager.hpp"

namespace script {

void barrelScript(AnimatedObject &animated, const GeneralContext &ctx) {
  if (InputManager::getInstance().isJustPressed("changeGun")) {
    animated.animator.play("idle_once");
    animated.animator.play("roll_once");
    GameState::getInstance().changeWeaponSelection();
  }
}
} // namespace script