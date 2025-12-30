#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "GeneralContext.hpp"
#include "RenderableObject.hpp"

class SceneManager {
public:
  using SceneSetupFn = std::function<void()>;

  static SceneManager &getInstance();

  void registerScene(const std::string &name, const SceneSetupFn &setup);
  void loadScene(const std::string &name);
  void reloadCurrentScene();
  bool isTransitioning();

  void setContext(GeneralContext &newContext);
  [[nodiscard]] const GeneralContext &getContext() const;

  void update();

private:
  SceneManager() = default;

  void beginTransition(const std::string &nextScene);
  void unloadCurrentScene();
  void initFadeOverlay();

  std::unordered_map<std::string, SceneSetupFn> scenes;
  std::string currentScene;
  std::string queuedScene;

  bool transitioning = false;
  bool fadingOut = true;

  float transitionTimer = 0.f;
  float transitionDuration = 0.5f;

  RenderableObject *fadeOverlay = nullptr;

  GeneralContext currentContext;
};