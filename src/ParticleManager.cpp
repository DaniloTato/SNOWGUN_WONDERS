#include "ParticleManager.hpp"
#include "Constants.hpp"
#include "PolyRenderizer.hpp"
#include "RenderCommand.hpp"
#include <cmath>
#include <algorithm>

ParticleManager::ParticleManager(const RenderizerParameters& params): renderizer(params){
    Renderizer::registerPair(this, &renderizer);
}

ParticleManager& ParticleManager::getInstance(const RenderizerParameters& params) {
    static ParticleManager instance(params);
    return instance;
}

void ParticleManager::update(const GeneralContext& ctx){
    updateParticles();
    updateRenderCommandBuffer();
    renderizer.updateRenderCommands(renderCommandBuffer);
}

void ParticleManager::emitSnow(const sf::Vector2f& pos) {
    Particle p;
    p.type = Type::Snow;
    p.pos = pos;

    p.vel = { (float)(rand()%20 - 10) * 0.1f, 30.f };

    p.gravity = 5.f;

    p.sinAmplitude = 1.f;
    p.sinFrequency = 5.f;
    p.sinPhase = (float)(rand() % 100) * 0.01f;

    p.shakeIntensity = 0.f;
    p.color = sf::Color(255,255,255,255);

    p.texRect = sf::IntRect(0,0,1,1);

    p.maxLifetime = 7.f;
    p.lifetime = p.maxLifetime;

    particles.push_back(p);
}

void ParticleManager::emitDust(const sf::Vector2f& pos) {
    Particle p;
    p.type = Type::Dust;
    p.pos = pos;

    p.vel = { (float)(rand()%50 - 25), (float)(-(rand()%50)) };
    p.gravity = 50.f;

    p.sinAmplitude = 0.f;
    p.sinFrequency = 0.f;
    p.sinPhase = 0.f;

    p.shakeIntensity = 3.f;

    p.color = sf::Color(200,200,200,150);
    p.texRect = sf::IntRect(16,0,8,8);

    p.maxLifetime = 1.5f;
    p.lifetime = p.maxLifetime;

    particles.push_back(p);
}

void ParticleManager::setWind(const sf::Vector2f& windVec) {
    wind = windVec;
}

void ParticleManager::updateParticles() {

    float dt = 1.f/Constants::FRAME_RATE;

    for (auto& p : particles) {
        p.lifetime -= dt;
        if (p.lifetime <= 0.f) continue;

        p.vel += wind * dt;
        p.vel.y += p.gravity * dt;
        p.pos += p.vel * dt;

        if (p.type == Type::Snow) {
            float offset = std::sin((p.maxLifetime - p.lifetime) * p.sinFrequency + p.sinPhase) 
                           * p.sinAmplitude;
            p.pos.x += offset * dt * 60.f;
        }
    }

    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
            [](const Particle& p){ return p.lifetime <= 0.f; }),
        particles.end()
    );
}


void ParticleManager::updateRenderCommandBuffer() {

    renderCommandBuffer.clear();

    for (auto& p : particles) {
        RenderCommand cmd;
        cmd.rect = p.texRect;
        cmd.pos = p.pos;
        cmd.color = p.color;

        if (p.type == Type::Dust && p.shakeIntensity > 0.f) {
            float t = (p.maxLifetime - p.lifetime) * 30.f;
            cmd.pos.x += std::sin(t * 2.7f) * p.shakeIntensity;
            cmd.pos.y += std::cos(t * 3.7f) * p.shakeIntensity;
        }

        renderCommandBuffer.push_back(cmd);
    }
}

void ParticleManager::destroyAll() {
    particles.clear();
}