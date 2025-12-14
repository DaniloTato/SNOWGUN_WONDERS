/*SFML dependency*/
#include <SFML/Graphics.hpp>

#include "BulletManager.hpp"
#include "GeneralContext.hpp"

/*Engine Objects*/
#include "GameCamera.hpp"
#include "GameObject.hpp"
#include "LevelManager.hpp"
#include "ParticleManager.hpp"
#include "PolyRenderizer.hpp"
#include "RenderableObject.hpp"
#include "Renderizer.hpp"
#include "SFML/System/Vector2.hpp"
#include "TangibleObject.hpp"
#include "InputManager.hpp"
#include "GameState.hpp"
#include "ScriptRunner.hpp"
#include "DialogueManager.hpp"

/*Namespaces*/
#include "Constants.hpp"
#include "ColorPalette.hpp"

/*Camera Scripts*/
#include "cameraShake.hpp"
#include "dramaticZoom.hpp"
#include "enemyPatrol.hpp"
#include "followPlayer.hpp"

/*TangibleObject Scripts*/
#include "movement.hpp"
#include "particleGeneration.hpp"
#include "tangibleAnimations.hpp"

/*General Scrits*/
#include "levelCreatorInputs.hpp"
#include "particleGeneration.hpp"
#include "toddTalk.hpp"

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ctime>

int main() {

    InputManager& inputManager = InputManager::getInstance();
    LevelManager& levelManager = LevelManager::getInstance();

    sf::RenderWindow window(sf::VideoMode(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT), "SFML Window");
    window.setFramerateLimit(Constants::FRAME_RATE);

    sf::Clock clock;

    inputManager.loadBindingsFromJsonFile("./config/control_config.json");

    GameState::getInstance().getMainCamera()->zoomTo(3.f);
    GameState::getInstance().getMainCamera() -> scripter.addScript(script::followPlayer);
    GameState::getInstance().getMainCamera() -> scripter.addScript(script::dramaticZoom);
    GameState::getInstance().getMainCamera() -> scripter.addScript(script::cameraShake);

    ScriptRunner scriptRunner;
    scriptRunner.scripter.addScript(script::levelCreatorInputs);
    scriptRunner.scripter.addScript(script::particleGeneration);

    /*Particles*/
    sf::Texture snowTexture;
    snowTexture.loadFromFile("assets/snow.png");
        //as of now. Particles ignore texture, rect, position and parallax
    RenderizerParameters particleParams{
        window,
        snowTexture,
        sf::IntRect(0,0,0,0),
        {0.f, 0.f},
        GameState::getInstance().getMainCamera(),
        -1.f,
        0.7f
    };

    PolyRenderizer particleRenderizer(particleParams);
    ParticleManager& particleManager = ParticleManager::getInstance();
    particleManager.attachPolyRederizer(&particleRenderizer); //Imperative attatchment of rendenderizer.
    /*Particles*/

    /*Dialogues*/
    sf::Texture fontTexture;
    fontTexture.loadFromFile("assets/font.png");
    RenderizerParameters textParams{
        window,
        fontTexture,
        sf::IntRect(),
        {0.f, 0.f},
        GameState::getInstance().getMainCamera(),
        -2.f,
        1.f
    };
    DialogueManager& dialogueManager = DialogueManager::getInstance();
    dialogueManager.attachTextParams(&textParams);
    dialogueManager.loadDialoguesFromFile("assets/dialogues/dialogues.txt");
    /*Dialogues*/

    sf::Texture toddTexture;
    toddTexture.loadFromFile("assets/todd.png");

    RenderizerParameters toddParams{
        window,
        toddTexture,
        {0,0,16,16},
        {128.f, 0.f},
        GameState::getInstance().getMainCamera(),
        0.f,
        1.f
    };
    RenderableObject todd(toddParams);
    todd.scripter.addScript(script::toddTalk);
    dialogueManager.assignDialogue(&todd, "Greeting");

    /*Enemy*/
    sf::Texture toyTexture;
    toyTexture.loadFromFile("assets/toy.png");

    RenderizerParameters toyParams{
        window,
        toyTexture,
        {0,0,17,17},
        {16.f, 16.f},
        GameState::getInstance().getMainCamera(),
        0.f,
        1.f
    };

    TangibleObject toy(toyParams);
    toy.collider.setOffset({5.f, 2.f});
    toy.collider.setSize({15.f, 14.f});

    toy.animator.loadFromAsepriteJSON("assets/json/toy.json");

    toy.scripter.addScript(script::enemyPatrol);
    /*Enemy*/

    /*Player*/
    sf::Texture playerTexture;
    playerTexture.loadFromFile("assets/snowman_animation.png");

    RenderizerParameters playerParams{
        window,
        playerTexture,
        {0,0,17,17},
        {16.f, 16.f},
        GameState::getInstance().getMainCamera(),
        0.f,
        1.f
    };

    TangibleObject player(playerParams);
    player.collider.setSize({18.f, 18.f});

    player.scripter.addScript(script::tangibleAnimations);
    player.scripter.addScript(script::movement);

    player.animator.loadFromAsepriteJSON("assets/json/snowman_animation.json");
    player.animator.setSpeedMultiplier(1.8f);
    /*Player*/

    /*Bullet*/
    sf::Texture bulletTexture;
    bulletTexture.loadFromFile("assets/bullet.png");
    BulletManager& bulletManager = BulletManager::getInstance();
    /*Bullet*/

    levelManager.loadLevel(window, GameState::getInstance().getMainCamera(), "assets/level_data/level.json");

    while (window.isOpen()) {
        sf::Event event;

        inputManager.update();
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            inputManager.handleEvent(event);
        }

        //yet to modify general context
        GeneralContext ctx = {
            player.position,
            window,
            textParams,
            &player,
            bulletTexture
        };

        levelManager.applyQueuedTileChanges();
        dialogueManager.applyQueues();
        bulletManager.getInstance().applyQueues();

        window.clear(ColorPalette::Black);

        for (GameObject* gameObject :  GameObject::getGameObjects()) {
            if (!gameObject) {
                std::cerr << "[Warning] Null GameObject pointer encountered during update. Skipping.\n";
                continue;
            }
            gameObject->update(ctx);
        }

        Renderizer::renderAll();

        toy.collider.debugRender(window, *GameState::getInstance().getMainCamera(), toy.position);

        window.display();
    }
    
    return 0;
}