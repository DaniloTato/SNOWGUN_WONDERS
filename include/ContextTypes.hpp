#pragma once

#include <variant>
#include <SFML/System/Vector2.hpp>  // for sf::Vector2f

struct BaseContext {
    virtual ~BaseContext() = default;
};

struct TangibleContext {
    sf::Vector2f position = {0.f, 0.f};
};

struct CameraContext{
    sf::Vector2f position = {0.f, 0.f};
};

using GameContext = std::variant<BaseContext, TangibleContext, CameraContext>;