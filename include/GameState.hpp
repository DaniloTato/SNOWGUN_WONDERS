#pragma once

#include <vector>
#include "GameCamera.hpp"
#include "GameObject.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

class GameState {
public:

    enum class CameraList{
        MAIN,
        UI    
    };

    static GameState& getInstance();

    void addCamera(GameCamera* camera);
    void removeCamera(GameCamera* camera);
    const std::vector<GameCamera*>& getActiveCameras() const;
    GameCamera* getMainCamera() const;
    GameCamera* getUiCamera() const;
    sf::RenderWindow* getMainWindow() const;
    void clearCameras();
    const float dt();

private:
    GameState();
    ~GameState() = default;

    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;

    GameState(GameState&&) = delete;
    GameState& operator=(GameState&&) = delete;

    std::vector<GameCamera*> activeCameras;
    std::vector<sf::RenderWindow*> activeWindows;
};