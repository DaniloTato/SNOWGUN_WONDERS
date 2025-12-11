/*SFML dependency*/
#include <SFML/Graphics.hpp>

#include "GeneralContext.hpp"

/*Engine Objects*/
#include "GameCamera.hpp"
#include "GameObject.hpp"
#include "LevelManager.hpp"
#include "Renderizer.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "TangibleObject.hpp"
#include "InputManager.hpp"
#include "Constants.hpp"
#include "GameState.hpp"
#include "TilePicker.hpp"

/*Camera Scripts*/
#include "cameraShake.hpp"
#include "dramaticZoom.hpp"
#include "followPlayer.hpp"

/*TangibleObject Scripts*/
#include "movement.hpp"
#include "tangibleAnimations.hpp"

#include <iostream>

int main() {

    InputManager& inputManager = InputManager::getInstance();

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

    player.animator.loadFromAsepriteJSON("assets/json/snowman_animation.json");
    player.animator.setSpeedMultiplier(1.8f);

    LevelManager::getInstance().loadLevel(window, GameState::getInstance().getMainCamera(), "assets/level_data/level.json");

    while (window.isOpen()) {
        sf::Event event;

        inputManager.update();
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            inputManager.handleEvent(event);
        }

        /*Level Creator Inputs*/

        if (inputManager.isJustPressed("tilePicker")){
            TilePicker picker(
                LevelManager::getInstance().getTilesheet(),
                Constants::TILE_SIZE
            );

            LevelManager::getInstance().selectedTileRect = picker.open(LevelManager::getInstance().layers, LevelManager::getInstance().activeLayer);
            LevelManager::getInstance().reloadAllLayers(window, GameState::getInstance().getMainCamera());
        }

        if(inputManager.isPressed("createTile")){
            sf::Vector2f mousePosToTilePos = GameState::getInstance().getMainCamera()->screenToWorld(
                {static_cast<float>(inputManager.getMousePosition().x), static_cast<float>(inputManager.getMousePosition().y)},
                LevelManager::getInstance().getLayerInfo(LevelManager::getInstance().activeLayer).paralax
            );

            LevelManager::getInstance().createTile(
                window,
                GameState::getInstance().getMainCamera(),
                LevelManager::getInstance().activeLayer,
                static_cast<int>(mousePosToTilePos.x) / Constants::TILE_SIZE,
                static_cast<int>(mousePosToTilePos.y) / Constants::TILE_SIZE,
                LevelManager::getInstance().selectedTileRect
            );
        }

        if(inputManager.isPressed("deleteTile")){
            sf::Vector2f mousePosToTilePos = GameState::getInstance().getMainCamera()->screenToWorld(
                {static_cast<float>(inputManager.getMousePosition().x), static_cast<float>(inputManager.getMousePosition().y)},
                LevelManager::getInstance().getLayerInfo(LevelManager::getInstance().activeLayer).paralax
            );

            LevelManager::getInstance().deleteTile(
                LevelManager::getInstance().activeLayer,
                static_cast<int>(mousePosToTilePos.x) / Constants::TILE_SIZE,
                static_cast<int>(mousePosToTilePos.y) / Constants::TILE_SIZE
            );
        }

        if(inputManager.isJustPressed("saveLevel")){
            std::cout << "Saving level..." << std::endl;
            LevelManager::getInstance().saveLevel("assets/level_data/level.json");
        }

        /*Level Creator Inputs*/

        GeneralContext ctx;
        ctx.playerPosition = player.position;

        window.clear();

        for (GameObject* gameObject : GameObject::getGameObjects()) {
            gameObject->update(ctx);
        }
        Renderizer::renderAll();

        window.display();
    }
    
    return 0;
}