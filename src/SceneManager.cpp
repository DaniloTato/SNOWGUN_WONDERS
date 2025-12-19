#include "SceneManager.hpp"
#include "GameObject.hpp"
#include "LevelManager.hpp"
#include "GameState.hpp"

SceneManager& SceneManager::getInstance() {
    static SceneManager instance;
    return instance;
}

void SceneManager::registerScene(const std::string& name, SceneSetupFn setup) {
    scenes[name] = setup;
}

void SceneManager::loadScene(const std::string& name) {
    if (transitioning) return;
    beginTransition(name);
}

void SceneManager::beginTransition(const std::string& nextScene) {
    transitioning = true;
    transitionTimer = 0.f;
    queuedScene = nextScene;
}

void SceneManager::finishTransition() {
    GameObject::destroyAll();

    auto& levelManager = LevelManager::getInstance();
    levelManager = LevelManager();

    scenes[queuedScene]();
    currentScene = queuedScene;

    transitioning = false;
}

void SceneManager::update() {
    if (!transitioning) return;

    transitionTimer += GameState::getInstance().dt();

    if (transitionTimer >= 0.5f) {
        finishTransition();
    }
}