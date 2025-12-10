#include "TilePicker.hpp"
#include <SFML/Graphics.hpp>

TilePicker::TilePicker(sf::Texture& tileset, int tileSize)
    : tileset(tileset), tileSize(tileSize) {}

sf::IntRect TilePicker::open()
{
    sf::RenderWindow window(sf::VideoMode(tileset.getSize().x, tileset.getSize().y), "Select Tile");

    sf::Sprite sheetSprite(tileset);

    while (window.isOpen())
    {
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
                window.close();

            if (ev.type == sf::Event::MouseButtonPressed)
            {
                int mx = ev.mouseButton.x;
                int my = ev.mouseButton.y;

                int tileX = mx / tileSize;
                int tileY = my / tileSize;

                int texX = tileX * tileSize;
                int texY = tileY * tileSize;

                window.close();
                return sf::IntRect(texX, texY, tileSize, tileSize);
            }
        }

        sf::Vector2i mp = sf::Mouse::getPosition(window);
        int hoverX = mp.x / tileSize;
        int hoverY = mp.y / tileSize;

        sf::RectangleShape highlight(sf::Vector2f(tileSize, tileSize));
        highlight.setPosition(hoverX * tileSize, hoverY * tileSize);
        highlight.setFillColor(sf::Color(0, 0, 0, 0));  // transparent fill
        highlight.setOutlineThickness(2);
        highlight.setOutlineColor(sf::Color::Yellow);

        std::vector<sf::Vertex> gridLines;

        int sheetW = tileset.getSize().x;
        int sheetH = tileset.getSize().y;

        int cols = sheetW / tileSize;
        int rows = sheetH / tileSize;

        for (int x = 0; x <= cols; x++)
        {
            gridLines.push_back(sf::Vertex(sf::Vector2f(x * tileSize, 0), sf::Color(180,180,180)));
            gridLines.push_back(sf::Vertex(sf::Vector2f(x * tileSize, rows * tileSize), sf::Color(180,180,180)));
        }

        for (int y = 0; y <= rows; y++)
        {
            gridLines.push_back(sf::Vertex(sf::Vector2f(0, y * tileSize), sf::Color(180,180,180)));
            gridLines.push_back(sf::Vertex(sf::Vector2f(cols * tileSize, y * tileSize), sf::Color(180,180,180)));
        }

        window.clear(sf::Color::Black);
        window.draw(sheetSprite);
        window.draw(&gridLines[0], gridLines.size(), sf::Lines);
        window.draw(highlight);
        window.display();
    }

    return sf::IntRect(0, 0, tileSize, tileSize);
}