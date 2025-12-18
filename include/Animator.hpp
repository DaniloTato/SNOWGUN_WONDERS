#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <string>

class Animator {
public:
    struct Frame {
        sf::IntRect rect;
        float duration;
    };

    struct Animation {
        std::string name;
        std::vector<Frame> frames;
        bool loop = true;
    };

    Animator();

    void addAnimation(const std::string& name, const Animation& anim);
    void setAnimations(std::unordered_map<std::string, Animation>& newAnimations);
    void update();

    void play(const std::string& name);

    void setSpeedMultiplier(float multiplier);

    void loadAsepriteAnimations(const std::string& filename);
    static std::unordered_map<std::string, Animation> getAsepriteJSONAnimations(const std::string& filename);

    const sf::IntRect& getCurrentFrame() const;

    bool animationFinished() const;

private:
    std::unordered_map<std::string, Animation> animations;

    const Animation* currentAnim = nullptr;
    std::string currentState;

    float timer = 0.f;
    std::optional<std::string> current;
    size_t index = 0;

    float speedMultiplier = 1.f;
    bool finished = false;
};