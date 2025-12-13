#include "Animator.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

Animator::Animator() {}

using json = nlohmann::json;

void Animator::loadFromAsepriteJSON(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[Animator] ERROR: Could not open JSON: " << filename << "\n";
        return;
    }

    json j;
    file >> j;

    std::vector<sf::IntRect> allFrames;
    std::vector<float> allDurations;

    for (auto& [key, frameData] : j["frames"].items()) {
        auto f = frameData["frame"];
        allFrames.push_back(sf::IntRect(f["x"], f["y"], f["w"], f["h"]));
        allDurations.push_back(frameData["duration"].get<float>() / 1000.f);
    }

    for (auto& tag : j["meta"]["frameTags"]) {
        Animation anim;
        anim.loop = tag["name"].get<std::string>().find("_once") == std::string::npos;

        int from = tag["from"];
        int to   = tag["to"];

        float totalTime = 0.f;
        for (int i = from; i <= to; i++)
            totalTime += allDurations[i];

        anim.frameTime = totalTime / (to - from + 1);

        for (int i = from; i <= to; i++)
            anim.frames.push_back(allFrames[i]);

        std::string name = tag["name"];
        addAnimation(name, anim);
    }
}

void Animator::addAnimation(const std::string& name, const Animation& anim) {
    animations[name] = anim;

    if (!currentAnim) {
        currentAnim = &animations[name];
        currentState = name;
        currentFrame = 0;
    }
}

void Animator::setState(const std::string& name) {
    if (currentState == name) return;

    auto it = animations.find(name);
    if (it == animations.end()) return;

    currentAnim = &it->second;
    currentState = name;
    currentFrame = 0;
    timer = 0.f;
    finished = false;
}

void Animator::update(float dt) {
    if (!currentAnim || finished) return;

    timer += dt * speedMultiplier;

    if (timer >= currentAnim->frameTime) {
        timer = 0.f;

        if (currentFrame + 1 < currentAnim->frames.size()) {
            currentFrame++;
        }
        else {
            if (currentAnim->loop) {
                currentFrame = 0;
            } else {
                finished = true;
            }
        }
    }
}

const sf::IntRect& Animator::getCurrentFrame() const {
    return currentAnim->frames[currentFrame];
}

void Animator::setSpeedMultiplier(float multiplier) {
    speedMultiplier = multiplier;
}

bool Animator::animationFinished() const {
    return finished;
}