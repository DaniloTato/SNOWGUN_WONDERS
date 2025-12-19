#pragma once
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <SFML/Graphics.hpp>
#include "GameCamera.hpp"

struct RenderizerParameters;

struct RenderEntry {
    GameObject* object;
    class Renderizer* renderizer;
    bool isRectShape = false;
    
};

class Renderizer {
public:
    Renderizer(const RenderizerParameters& params);
    virtual ~Renderizer();

    void setRect(const sf::IntRect& newRect, int direction);
    const sf::IntRect& getRect() const;
    virtual void render(GameObject* ob);
    void renderRectShape(GameObject* obj);
    void assignCamera(GameCamera* cam);
    const float getLayer() const;
    void setColor(sf::Color newColor);
    void toggleShowEvery(float time);
    bool shouldIRender();

    static void registerPair(GameObject* obj, Renderizer* rend, bool isRectShape);
    static void unregisterPair(Renderizer* rend);

    static void renderAll();

protected:
    sf::RenderWindow& window;
    sf::Sprite sprite;
    sf::Texture& texture;
    sf::IntRect rect;
    sf::Color color;
    GameCamera* assignedCamera;
    float layer;
    float paralax;

    bool show;
    float showCountDown;

    static std::vector<RenderEntry> registry;
};