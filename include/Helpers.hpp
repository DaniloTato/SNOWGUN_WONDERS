#pragma once

#include <SFML/Graphics.hpp>

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
}