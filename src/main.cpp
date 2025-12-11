/*SFML dependency*/
#include <SFML/Graphics.hpp>

#include "GeneralContext.hpp"

/*Engine Objects*/
#include "GameCamera.hpp"
#include "GameObject.hpp"
#include "LevelManager.hpp"
#include "Renderizer.hpp"
#include "TangibleObject.hpp"
#include "InputManager.hpp"
#include "GameState.hpp"
#include "ScriptRunner.hpp"

/*Namespaces*/
#include "Constants.hpp"
#include "ColorPalette.hpp"

/*Camera Scripts*/
#include "cameraShake.hpp"
#include "dramaticZoom.hpp"
#include "followPlayer.hpp"

/*TangibleObject Scripts*/
#include "movement.hpp"
#include "tangibleAnimations.hpp"

/*General Scrits*/
#include "levelCreatorInputs.hpp"

int main() {

    InputManager& inputManager = InputManager::getInstance();
    LevelManager& levelManager = LevelManager::getInstance();

    sf::RenderWindow window(sf::VideoMode(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT), "SFML Window");
    window.setFramerateLimit(Constants::FRAME_RATE);

    inputManager.loadBindingsFromJsonFile("./config/control_config.json");

    sf::Texture playerTexture;
    playerTexture.loadFromFile("assets/snowman_animation.png");

    sf::IntRect playerRect(0, 0, 17, 17);

    GameState::getInstance().getMainCamera()->zoomTo(3.f);
    GameState::getInstance().getMainCamera() -> scripter.addScript(script::followPlayer);
    GameState::getInstance().getMainCamera() -> scripter.addScript(script::dramaticZoom);
    GameState::getInstance().getMainCamera() -> scripter.addScript(script::cameraShake);

    RenderizerParameters params{
        window,
        playerTexture,
        playerRect,
        {16.f, 16.f},
        GameState::getInstance().getMainCamera(),
        0.f,
        1.f
    };

    TangibleObject player(params);
    player.collider.setSize({18.f, 18.f});

    player.scripter.addScript(script::tangibleAnimations);
    player.scripter.addScript(script::movement);

    ScriptRunner scriptRunner;
    scriptRunner.scripter.addScript(script::levelCreatorInputs);

    player.animator.loadFromAsepriteJSON("assets/json/snowman_animation.json");
    player.animator.setSpeedMultiplier(1.8f);

    levelManager.loadLevel(window, GameState::getInstance().getMainCamera(), "assets/level_data/level.json");

    while (window.isOpen()) {
        sf::Event event;

        inputManager.update();
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            inputManager.handleEvent(event);
        }

        GeneralContext ctx = {
        player.position,
        window
        };

        window.clear(ColorPalette::ElectricBlue);

        for (GameObject* gameObject : GameObject::getGameObjects()) {
            gameObject->update(ctx);
        }
        Renderizer::renderAll();

        window.display();
    }
    
    return 0;
}