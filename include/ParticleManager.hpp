#pragma once
#include <limits>
#include <vector>
#include <SFML/Graphics.hpp>

#include "GameObject.hpp"
#include "PolyRenderizer.hpp"
#include "RenderCommand.hpp"

class ParticleManager: GameObject {
public:
    enum class Type {
        Snow,
        Dust
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

        sf::Color color;
        sf::IntRect texRect;
    };

    static ParticleManager& getInstance();

    void emitSnow(const sf::Vector2f& pos);
    void emitDust(const sf::Vector2f& pos);

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

    std::vector<RenderCommand> renderCommandBuffer;
};