#pragma once

#include <SFML/Graphics.hpp>

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

#include <filesystem>

namespace Helper{

    struct TriggerOnce {
        bool fired = false;

        template<typename Fn>
        void check(bool condition, Fn&& fn) {
            if (condition && !fired) {
                fired = true;
                fn();
            }
        }
    };

    template<typename T>
    constexpr T lerp(const T& actual, const T& desired, float speed) {
        return actual + (desired - actual) * speed;
    }

    sf::Texture& loadTexture(const std::string& path);
    float distance(const sf::Vector2f& pos1, const sf::Vector2f& pos2);
    sf::FloatRect makeRectFromPoints(float x1, float y1,float x2, float y2);
    std::filesystem::path getExecutableDir();
}