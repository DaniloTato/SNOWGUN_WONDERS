#pragma once

#include <vector>
#include "GameCamera.hpp"

class GameState {
public:
    static GameState& getInstance();

    void addCamera(GameCamera* camera);
    void removeCamera(GameCamera* camera);
    const std::vector<GameCamera*>& getActiveCameras() const;
    GameCamera* getMainCamera() const;
    void clearCameras();

private:
    GameState();
    ~GameState() = default;

    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;

    GameState(GameState&&) = delete;
    GameState& operator=(GameState&&) = delete;

    std::vector<GameCamera*> activeCameras;
};