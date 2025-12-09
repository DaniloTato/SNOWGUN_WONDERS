#pragma once
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <SFML/Graphics.hpp>
#include "GameCamera.hpp"

struct RenderizerParameters {
    sf::RenderWindow& window;
    sf::Texture& texture;
    sf::IntRect& rect;
    sf::Vector2f position = {0.f, 0.f};
    GameCamera* camera = nullptr;
};

class Renderizer {
public:
    Renderizer(const RenderizerParameters& params);
    ~Renderizer() = default;

    void setRect(const sf::IntRect& newRect, int direction);
    void render(sf::Vector2f position);
    void assignCamera(GameCamera* cam);

private:
    sf::RenderWindow& window;
    sf::Sprite sprite;
    sf::Texture texture;
    sf::IntRect rect;
    GameCamera* assignedCamera;
};