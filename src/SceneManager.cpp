#include "SceneManager.hpp"
#include "GameObject.hpp"
#include "LevelManager.hpp"
#include "GameState.hpp"
#include "BulletManager.hpp"
#include "EnemyManager.hpp"
#include "ParticleManager.hpp"
#include "DialogueManager.hpp"

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

void SceneManager::unloadCurrentScene() {

    BulletManager::getInstance().onSceneUnload();
    EnemyManager::getInstance().onSceneUnload();
    ParticleManager::getInstance().onSceneUnload();
    DialogueManager::getInstance().onSceneUnload();
    LevelManager::getInstance().onSceneUnload();

    GameState::getInstance().clearCameras();

    GameObject::destroySceneObjects();
}

void SceneManager::finishTransition() {
    unloadCurrentScene();

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

bool SceneManager::isTransitioning(){
    return transitioning;
}

void SceneManager::setContext(GeneralContext& newContext){
    currentContext = newContext;
}

const GeneralContext& SceneManager::getContext() const{
    return currentContext;
}