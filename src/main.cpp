/*SFML dependency*/
#include <SFML/Graphics.hpp>

/*Engine Objects*/
#include "GameCamera.hpp"
#include "GameObject.hpp"
#include "LevelManager.hpp"
#include "RenderableObject.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "TangibleObject.hpp"
#include "InputManager.hpp"
#include "Constants.hpp"
#include "GameState.hpp"

/*Camera Scripts*/
#include "scripts/camera/followPlayer.cpp"
#include "scripts/camera/dramaticZoom.cpp"
#include "scripts/camera/cameraShake.cpp"

int main() {

    sf::RenderWindow window(sf::VideoMode(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT), "SFML Window");
    window.setFramerateLimit(Constants::FRAME_RATE);
    InputManager::getInstance().loadBindingsFromJsonFile("./config/control_config.json");

    sf::Texture playerTexture;
    playerTexture.loadFromFile("assets/snowman.png");

    sf::IntRect playerRect(0, 0, 17, 17);

    GameState::getInstance().getMainCamera() -> scripter.addScript(script::followPlayer);
    GameState::getInstance().getMainCamera() -> scripter.addScript(script::dramaticZoom);
    GameState::getInstance().getMainCamera() -> scripter.addScript(script::cameraShake);

    RenderizerParameters params{
        window,
        playerTexture,
        playerRect,
        {16.f, 16.f},
        GameState::getInstance().getMainCamera()
    };

    TangibleObject player(params);
    player.collider.setSize({16.f, 16.f});

    LevelManager::getInstance().loadLevel(window, GameState::getInstance().getMainCamera(), "assets/level_data/level.txt");

    while (window.isOpen()) {
        sf::Event event;

        InputManager::getInstance().update();
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            InputManager::getInstance().handleEvent(event);
        }

        CameraContext camCtx;
        camCtx.position = player.getPosition();

        for (GameCamera* gameCamera : GameState::getInstance().getActiveCameras()) {
            gameCamera->update(camCtx);
        }

        window.clear();
        for (GameObject* gameObject : GameObject::getGameObjects()) {
            gameObject->update();
        }
        window.display();
    }
    
    return 0;
}