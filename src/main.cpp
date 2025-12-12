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
#include "GameText.hpp"

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

#include <iostream>
#include <ctime>

int main() {

    InputManager& inputManager = InputManager::getInstance();
    LevelManager& levelManager = LevelManager::getInstance();

    sf::RenderWindow window(sf::VideoMode(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT), "SFML Window");
    window.setFramerateLimit(Constants::FRAME_RATE);

    sf::Clock clock;

    inputManager.loadBindingsFromJsonFile("./config/control_config.json");

    sf::Texture playerTexture;
    playerTexture.loadFromFile("assets/snowman_animation.png");

    sf::IntRect playerRect(0, 0, 17, 17);

    GameState::getInstance().getMainCamera()->zoomTo(3.f);
    GameState::getInstance().getMainCamera() -> scripter.addScript(script::followPlayer);
    GameState::getInstance().getMainCamera() -> scripter.addScript(script::dramaticZoom);
    GameState::getInstance().getMainCamera() -> scripter.addScript(script::cameraShake);

    /*GameText*/
    sf::Texture fontTexture;
    fontTexture.loadFromFile("assets/font.png");

    //May need to rework the structure of Renderizer Parameters
    //Or maybe create a TextRenderizerParameters struct appart.
    //In this case, rect and position params are unnecesary
    RenderizerParameters textParams{
        window,
        fontTexture,
        sf::IntRect(),
        {0.f, 0.f},
        GameState::getInstance().getMainCamera(),
        -1.f,
        1.f
    }; //layer negative so it appears in front of everything

    GameText text(textParams);
    text.setFontAtlas(fontTexture, 9, 8, 95, 32);
    std::string markup = R"(
        #position 0 0
        #boundary 500
        #effect typewriter 0.05
        #alignment left

        Hello <color=yellow><anim=sin>World</anim></color>!<ln>
        This is <anim=shake:2>shaky</anim> text.
    )";
    text.loadFromMarkup(markup);
    /*GameText*/

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

        levelManager.applyQueuedTileChanges();

        window.clear(ColorPalette::ElectricBlue);

        for (GameObject* gameObject :  GameObject::getGameObjects()) {
            if (!gameObject) {
                std::cerr << "[Warning] Null GameObject pointer encountered during update. Skipping.\n";
                continue;
            }
            gameObject->update(ctx);
        }

        Renderizer::renderAll();

        window.display();
    }
    
    return 0;
}