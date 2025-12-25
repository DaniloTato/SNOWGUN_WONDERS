#include "sceneHelperFunctions.hpp"
#include "GameState.hpp"
#include "dramaticZoom.hpp"
#include "cameraAlarm.hpp"
#include "movement.hpp"
#include "Constants.hpp"
#include "Helpers.hpp"

const std::filesystem::path ROOT = Helper::getExecutableDir();

namespace SceneBuilder{

    // Creates cameras and adds standard scripts
    void setupCameras(GameState& gameState) {
        gameState.createCamera(GameState::CameraList::MAIN);
        gameState.createCamera(GameState::CameraList::UI);

        auto mainCam = gameState.getMainCamera();
        mainCam->zoomTo(3.f);
        mainCam->scripter.addScript(script::dramaticZoom);
        mainCam->scripter.addScript(script::cameraAlarm);
    }

    // Create player object with common setup
    TangibleObject* createPlayer(sf::RenderWindow& window, sf::Texture& texture, GameCamera* camera, sf::Vector2f position) {
        RenderizerParameters params{
            window,
            texture,
            {0,0,17,17},
            position,
            camera,
            -0.1f,
            1.f
        };
        TangibleObject* player = new TangibleObject(params);
        player->collider.setSize({16.f, 16.f});
        player->collider.setOffset({1.f, 2.f});
        player->scripter.addScript(script::movement);
        player->animator.loadAsepriteAnimations(ROOT / "assets/json/snowman_animation.json");
        player->animator.setSpeedMultiplier(1.8f);

        if(!GameState::getInstance().hasCheckpoint()){
            GameState::getInstance().setCheckpoint(player->position);
        } else{
            player->position = GameState::getInstance().getCheckpoint();
        }

        return player;
    }

    // Setup particles
    PolyRenderizer* setupParticles(sf::RenderWindow& window, ParticleManager& particleManager, GameCamera* camera) {
        static sf::Texture& particleTexture = Helper::loadTexture(ROOT / "assets/particles.png");
        RenderizerParameters params{
            window,
            particleTexture,
            sf::IntRect(0,0,0,0),
            {0.f,0.f},
            camera,
            -10.f,
            0.7f
        };
        PolyRenderizer* pr = new PolyRenderizer(params);
        particleManager.attachPolyRederizer(pr);
        return pr;
    }

    // Setup text parameters and dialogue system
    RenderizerParameters* setupTextAndDialogue(sf::RenderWindow& window, DialogueManager& dialogueManager, GameCamera* camera) {
        static sf::Texture& fontTexture = Helper::loadTexture(ROOT / "assets/font.png");
        RenderizerParameters* params = new RenderizerParameters{
            window,
            fontTexture,
            sf::IntRect(), 
            {0.f, 0.f},
            camera,
            Constants::TEXT_LAYER,
            1.f
        };
        dialogueManager.attachTextParams(params);
        return params;
    }

}