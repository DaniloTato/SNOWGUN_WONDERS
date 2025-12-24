#include "Helpers.hpp"

namespace Helper{
    sf::Texture& loadTexture(const std::string& path) {
        static std::unordered_map<std::string, sf::Texture> cache;
        if (cache.find(path) == cache.end()) {
            sf::Texture tex;
            if (!tex.loadFromFile(path)) {
                throw std::runtime_error("Failed to load texture " + path);
            }
            cache[path] = std::move(tex);
        }
        return cache[path];
    }
}