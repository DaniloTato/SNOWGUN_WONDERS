#include "sceneHelperFunctions.hpp"
#include "dramaticZoom.hpp"
#include "cameraAlarm.hpp"
#include "movement.hpp"

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

    // Create and setup a texture from file, returning reference
    sf::Texture& loadTexture(const std::string& path) {
        static std::unordered_map<std::string, sf::Texture> cache;
        if (cache.find(path) == cache.end()) {
            sf::Texture tex;
            if (!tex.loadFromFile(path)) {
                throw std::runtime_error("Failed to load texture " + path);
            }
            cache[path] = std::move(tex);
        }
        return cache[path];
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
        player->animator.loadAsepriteAnimations("assets/json/snowman_animation.json");
        player->animator.setSpeedMultiplier(1.8f);
        return player;
    }

    // Setup particles
    PolyRenderizer* setupParticles(sf::RenderWindow& window, ParticleManager& particleManager, GameCamera* camera) {
        static sf::Texture& particleTexture = loadTexture("assets/particles.png");
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
        static sf::Texture& fontTexture = loadTexture("assets/font.png");
        RenderizerParameters* params = new RenderizerParameters{
            window,
            fontTexture,
            sf::IntRect(),
            {0.f, 0.f},
            camera,
            -2.f,
            1.f
        };
        dialogueManager.attachTextParams(params);
        return params;
    }

}