#pragma once
#include <SFML/Graphics.hpp>

class UISlider {
public:
    // position = top-left of the track
    // width = pixel width of the track
    // minValue / maxValue = range for the slider
    // boundValue = pointer to float to modify (can be nullptr)
    // font is kept for parity if you want to draw labels (not mandatory)
    UISlider(const sf::Vector2f& position,
             float width,
             float minValue,
             float maxValue,
             float* boundValue,
             sf::Font& font);

    // draw the track + knob
    void draw(sf::RenderWindow& window);

    // handle an sf::Event (press/release) and window for mouse coords
    void handleEvent(const sf::Event& ev, sf::RenderWindow& window);

    // set instantly without touching the bound pointer (still updates knob)
    void setValue(float v);

    // get current value (from bound pointer if present, else internal)
    float getValue() const;

    // bind (or rebind) the slider to another float pointer (can be nullptr)
    void bindTo(float* newBound);

private:
    static float clamp(float v, float a, float b) {
        return (v < a) ? a : ((v > b) ? b : v);
    }

    float getPercentage() const; // 0..1 (uses current value or 0 if unbound)

    sf::Vector2f pos;
    float width;
    float minValue;
    float maxValue;

    // pointer to external value (nullable)
    float* valuePtr;

    // if unbound we still store an internal value
    float internalValue;

    // visuals
    sf::RectangleShape track;
    sf::CircleShape knob;

    // dragging state
    bool dragging = false;

    // optional font (not used heavily here but available)
    sf::Font& fontRef;
};