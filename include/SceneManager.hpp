#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include "GeneralContext.hpp"

class SceneManager {
public:
    using SceneSetupFn = std::function<void()>;

    static SceneManager& getInstance();

    void registerScene(const std::string& name, SceneSetupFn setup);
    void loadScene(const std::string& name);
    bool isTransitioning();

    void setContext(GeneralContext& newContext);
    const GeneralContext& getContext() const;

    void update();

private:
    SceneManager() = default;

    void beginTransition(const std::string& nextScene);
    void finishTransition();
    void unloadCurrentScene();

    std::unordered_map<std::string, SceneSetupFn> scenes;

    std::string currentScene;
    std::string queuedScene;

    bool transitioning = false;
    float transitionTimer = 0.f;

    GeneralContext currentContext;
};