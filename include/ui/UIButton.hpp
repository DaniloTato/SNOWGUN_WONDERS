#pragma once
#include <SFML/Graphics.hpp>

class UIButton
{
public:
    UIButton(const sf::Vector2f& position,
             const sf::Vector2f& size,
             const std::string& text,
             sf::Font& font);

    void draw(sf::RenderWindow& window);
    bool isClicked(const sf::Event& event, sf::RenderWindow& window);

    void setText(const std::string& t) { label.setString(t); }
    void setPosition(const sf::Vector2f& p);

private:
    sf::RectangleShape box;
    sf::Text label;
};