#pragma once
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <SFML/Graphics.hpp>
#include "GameCamera.hpp"

struct RenderizerParameters;

struct RenderEntry {
    GameObject* object;
    class Renderizer* renderer;
};

class Renderizer {
public:
    Renderizer(const RenderizerParameters& params);
    ~Renderizer();

    void setRect(const sf::IntRect& newRect, int direction);
    const sf::IntRect& getRect() const;
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