#include "GameState.hpp"
#include "GameCamera.hpp"
#include <SFML/Graphics.hpp>
#include "Constants.hpp"

GameState::GameState()
: activeCameras({new GameCamera()}), activeWindows({new sf::RenderWindow(sf::VideoMode(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT), "SFML Window")}) {}

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

sf::RenderWindow* GameState::getMainWindow() const {
    if(!activeWindows.empty()){
        return activeWindows.front();
    }
    return nullptr;
}

void GameState::clearCameras() {
    activeCameras.clear();
}

const float GameState::dt(){
    return 1.f/Constants::FRAME_RATE;
}