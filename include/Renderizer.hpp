#pragma once
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <SFML/Graphics.hpp>
#include "GameCamera.hpp"

struct RenderizerParameters {
    sf::RenderWindow& window;
    sf::Texture& texture;
    sf::IntRect rect;
    sf::Vector2f position = {0.f, 0.f};
    GameCamera* camera = nullptr;
    float layer = 0.f;
    float paralax = 1.f;
};

struct RenderEntry {
    GameObject* object;
    class Renderizer* renderer;
};

class Renderizer {
public:
    Renderizer(const RenderizerParameters& params);
    ~Renderizer();

    void setRect(const sf::IntRect& newRect, int direction);
    virtual void render(GameObject* ob);
    void assignCamera(GameCamera* cam);
    const float getLayer() const;

    static void registerPair(GameObject* obj, Renderizer* rend);
    static void unregisterPair(Renderizer* rend);

    static void renderAll();

protected:
    sf::RenderWindow& window;
    sf::Sprite sprite;
    sf::Texture& texture;
    sf::IntRect rect;
    GameCamera* assignedCamera;
    float layer;
    float paralax;

    static std::vector<RenderEntry> registry;
};