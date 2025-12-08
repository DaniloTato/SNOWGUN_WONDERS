#include "GameState.hpp"
#include "GameCamera.hpp"

GameState::GameState()
: activeCameras({new GameCamera()}) {}

GameState& GameState::getInstance() {
    static GameState instance;
    return instance;
}

void GameState::addCamera(GameCamera* camera) {
    if (std::find(activeCameras.begin(), activeCameras.end(), camera) == activeCameras.end()) {
        activeCameras.push_back(camera);
    }
}

void GameState::removeCamera(GameCamera* camera) {
    activeCameras.erase(std::remove(activeCameras.begin(), activeCameras.end(), camera), activeCameras.end());
}

const std::vector<GameCamera*>& GameState::getActiveCameras() const {
    return activeCameras;
}

GameCamera* GameState::getMainCamera() const {
    if (!activeCameras.empty()) {
        return activeCameras.front();
    }
    return nullptr;
}

void GameState::clearCameras() {
    activeCameras.clear();
}