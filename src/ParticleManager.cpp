#include "ParticleManager.hpp"
#include "Animator.hpp"
#include "Constants.hpp"
#include "PolyRenderizer.hpp"
#include "RenderCommand.hpp"
#include "ColorPalette.hpp"
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <iostream>

static float _toggleRand(float min, float max) {
    return min + ((float)rand() / RAND_MAX) * (max - min);
}

ParticleManager::ParticleManager(){
    cachedAnimations = Animator::getAsepriteJSONAnimations("assets/json/particles.json");
}

ParticleManager& ParticleManager::getInstance() {
    static ParticleManager instance;
    return instance;
}

void ParticleManager::attachPolyRederizer(PolyRenderizer* polyRenderizer){
    attachedRenderizer = polyRenderizer;
    Renderizer::registerPair(this, polyRenderizer);
}

void ParticleManager::update(const GeneralContext& ctx){
    updateParticles();
    updateRenderCommandBuffer();

    if(attachedRenderizer){
        attachedRenderizer -> updateRenderCommands(renderCommandBuffer);
    } else{
        std::cout << "[ParticleManager] Warning: No Renderizer Attached.\n";
    }
}

void ParticleManager::emitSnow(const sf::Vector2f& pos) {
    Particle p;
    p.type = Type::Snow;
    p.pos = pos;

    p.vel = { (float)(rand()%20 - 10) * 0.1f, 30.f };

    p.gravity = 5.f;

    p.parallax = (5 + rand() % 5)*0.1;

    p.sinAmplitude = 1.f;
    p.sinFrequency = 5.f;
    p.sinPhase = (float)(rand() % 100) * 0.01f;

    p.shakeIntensity = 0.f;
    p.color = sf::Color(255,255,255,255);

    p.texRect = sf::IntRect(0,0,1,1);

    p.maxLifetime = 7.f;
    p.lifetime = p.maxLifetime;

    particles.push_back(std::move(p));
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

    particles.push_back(std::move(p));
}

void ParticleManager::emitExplosion(const sf::Vector2f& pos, int count)
{
    for (int i = 0; i < count; i++) {
        Particle p;
        p.type = Type::Explosion;
        p.pos = pos + sf::Vector2f(_toggleRand(-16, 16), _toggleRand(-16, 16));

        const auto& palette = ColorPalette::EXPLOSION_COLORS;
        p.color = *palette[rand() % palette.size()];
        
        p.animator = std::make_unique<Animator>();
        p.animator->setAnimations(cachedAnimations);
        p.animator->setState("small_explosion_once");
        p.texRect = sf::IntRect(8, 8, 8, 8);

        p.maxLifetime = 0.6f;
        p.lifetime = p.maxLifetime;

        particles.push_back(std::move(p));
    }
}

void ParticleManager::setWind(const sf::Vector2f& windVec) {
    wind = windVec;
}

void ParticleManager::updateParticles() {

    float dt = 1.f/Constants::FRAME_RATE;

    for (auto& p : particles) {

        if(p.animator){
            p.animator->update();
            p.texRect = p.animator->getCurrentFrame();
            
            if(p.animator->animationFinished()){
                p.forceDeath = true;
            }
        }

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

        if (p.type == Type::Explosion) {
            float alpha = p.lifetime / p.maxLifetime;
            p.color.a = static_cast<sf::Uint8>(255 * alpha);
        }
    }

    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
            [](const Particle& p){ return p.lifetime <= 0.f || p.forceDeath; }),
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
        cmd.overrideParalax = p.parallax;

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