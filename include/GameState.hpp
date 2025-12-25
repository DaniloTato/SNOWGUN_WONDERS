#pragma once

#include <vector>
#include <chrono>
#include "GameCamera.hpp"
#include "Bullet.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"

class GameState {
public:

    enum class CameraList{
        MAIN,
        UI,
        COUNT
    };

    static GameState& getInstance();

    void addCamera(GameCamera* camera);
    void removeCamera(GameCamera* camera);
    void createCamera(CameraList type);
    const std::vector<GameCamera*>& getActiveCameras() const;
    GameCamera* getMainCamera() const;
    GameCamera* getUiCamera() const;
    sf::RenderWindow* getMainWindow() const;
    void clearCameras();
    float dt();
    void updateDt();

    void setCheckpoint(sf::Vector2f position);
    const sf::Vector2f getCheckpoint() const;

    int getCrystalAmount() const;
    void addToCrystalAmount(int amount);

    int getPlayerHealth() const;
    void changePlayerHealth(int amount);

    void changeWeaponSelection();
    Bullet::Type getWeaponSelection();

    bool hasCheckpoint() const;

private:
    GameState();
    ~GameState() = default;

    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;

    GameState(GameState&&) = delete;
    GameState& operator=(GameState&&) = delete;

    std::vector<GameCamera*> activeCameras;
    std::vector<sf::RenderWindow*> activeWindows;

    std::chrono::high_resolution_clock::time_point lastFrameTime = std::chrono::high_resolution_clock::now();
    float dtValue;

    sf::Vector2f checkpoint;

    int crystals;
    int playerHealth;
    Bullet::Type selectedWeapon;
};