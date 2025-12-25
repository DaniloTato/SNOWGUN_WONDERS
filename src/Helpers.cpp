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

    float distance(const sf::Vector2f& a, const sf::Vector2f& b) {
        float dx = a.x - b.x;
        float dy = a.y - b.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    sf::FloatRect makeRectFromPoints(
    float x1, float y1,
    float x2, float y2
    ) {
        float left   = std::min(x1, x2);
        float top    = std::min(y1, y2);
        float width  = std::abs(x2 - x1);
        float height = std::abs(y2 - y1);

        return sf::FloatRect(left, top, width, height);
    }
}