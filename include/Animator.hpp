#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <string>

class Animator {
public:
    struct Animation {
        std::vector<sf::IntRect> frames;
        float frameTime;
        bool loop = true;
    };

    Animator();

    void addAnimation(const std::string& name, const Animation& anim);
    void setState(const std::string& name);
    void update(float dt);

    void setSpeedMultiplier(float multiplier);

    void loadFromAsepriteJSON(const std::string& filename);

    const sf::IntRect& getCurrentFrame() const;

private:
    std::unordered_map<std::string, Animation> animations;

    const Animation* currentAnim = nullptr;
    std::string currentState;

    float timer = 0.f;
    size_t currentFrame = 0;

    float speedMultiplier = 1.f;
};