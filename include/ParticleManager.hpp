#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

#include "Animator.hpp"
#include "GameObject.hpp"
#include "PolyRenderizer.hpp"
#include "RenderCommand.hpp"

class ParticleManager: GameObject {
public:
    enum class Type {
        Snow,
        Dust,
        Explosion,
        Stars
    };

    struct Particle {
        Type type;
        sf::Vector2f pos;
        sf::Vector2f vel;
        float lifetime;
        float maxLifetime;
        float gravity;

        float sinAmplitude;
        float sinFrequency;
        float sinPhase;

        float shakeIntensity;

        float parallax = 1.f;

        bool forceDeath = false;

        sf::Color color;
        sf::IntRect texRect;
        std::unique_ptr<Animator> animator;
    };

    static ParticleManager& getInstance();

    void emitSnow(const sf::Vector2f& pos);
    void emitDust(const sf::Vector2f& pos);
    void emitExplosion(const sf::Vector2f& pos, int count = 12);
    void emitStars(const sf::Vector2f& pos, int count = 1);

    void setWind(const sf::Vector2f& windVec);

    void updateParticles();
    void updateRenderCommandBuffer();

    virtual void update(const GeneralContext& ctx) override;

    void attachPolyRederizer(PolyRenderizer* polyRenderizer);

    void destroyAll();

private:
    ParticleManager();

    std::vector<Particle> particles;
    PolyRenderizer* attachedRenderizer;
    sf::Vector2f wind = {0.f, 0.f};

    std::unordered_map<std::string, Animator::Animation> cachedAnimations;

    std::vector<RenderCommand> renderCommandBuffer;
};