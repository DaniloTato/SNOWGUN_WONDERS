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
    void setAnimations(std::unordered_map<std::string, Animation>& newAnimations);
    void setState(const std::string& name);
    void update();

    void setSpeedMultiplier(float multiplier);

    void loadFromAsepriteJSON(const std::string& filename);
    static std::unordered_map<std::string, Animation> getAsepriteJSONAnimations(const std::string& filename);

    const sf::IntRect& getCurrentFrame() const;

    bool animationFinished() const;

private:
    std::unordered_map<std::string, Animation> animations;

    const Animation* currentAnim = nullptr;
    std::string currentState;

    float timer = 0.f;
    size_t currentFrame = 0;

    float speedMultiplier = 1.f;
    bool finished = false;
};