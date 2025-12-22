#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "GameState.hpp"
#include "TangibleObject.hpp"
#include "PolyRenderizer.hpp"
#include "ParticleManager.hpp"
#include "DialogueManager.hpp"

namespace SceneBuilder{
    void setupCameras(GameState& gameState);
    sf::Texture& loadTexture(const std::string& path);
    TangibleObject* createPlayer(sf::RenderWindow& window, sf::Texture& texture, GameCamera* camera, sf::Vector2f position);
    PolyRenderizer* setupParticles(sf::RenderWindow& window, ParticleManager& particleManager, GameCamera* camera);
    RenderizerParameters* setupTextAndDialogue(sf::RenderWindow& window, DialogueManager& dialogueManager, GameCamera* camera);
}