#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>

class SoundManager {
public:
    static SoundManager& getInstance() {
        static SoundManager instance;
        return instance;
    }

    const sf::SoundBuffer& load(const std::string& id, const std::string& path) {
        auto& buf = buffers[id];
        if (!buf.loadFromFile(path)) {
            throw std::runtime_error("Failed to load sound: " + path);
        }
        return buf;
    }

    const sf::SoundBuffer& get(const std::string& id) const {
        return buffers.at(id);
    }

private:
    std::unordered_map<std::string, sf::SoundBuffer> buffers;
};