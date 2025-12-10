#include "TilePicker.hpp"
#include "UIButton.hpp"
#include "UISlider.hpp"

TilePicker::TilePicker(sf::Texture& tileset, int tileSize)
    : tileset(tileset), tileSize(tileSize) {}

sf::IntRect TilePicker::open(std::vector<LayerInfo>& layers, int& activeLayer){
    unsigned int winW = unsigned(tileset.getSize().x + 260);
    unsigned int winH = unsigned(tileset.getSize().y + 150);

    sf::RenderWindow window(sf::VideoMode(winW, winH), "Tile Picker");
    window.setFramerateLimit(60);

    sf::Font font;
    font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf");

    sf::IntRect selectedRect(0, 0, tileSize, tileSize);

    UIButton addBtn(
        { float(tileset.getSize().x) + 10.f, 10.f },
        { 240.f, 24.f },
        "+ Add Layer",
        font
    );

    // Validate activeLayer
    if (layers.empty()) activeLayer = -1;
    else if (activeLayer < 0 || activeLayer >= (int)layers.size())
        activeLayer = 0;

    // Create a UISlider bound to the parallax value
    float dummyMin = 0.1f;
    float dummyMax = 3.0f;
    float* bound = (activeLayer >= 0 ? &layers[activeLayer].paralax : nullptr);

    UISlider parallaxSlider(
        { 10.f, float(tileset.getSize().y) + 50.f },
        200.f,
        dummyMin,
        dummyMax,
        bound,
        font
    );

    while (window.isOpen())
    {
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed) {
                window.close();
                break;
            }

            if (ev.type == sf::Event::MouseButtonPressed &&
                ev.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mp = sf::Mouse::getPosition(window);
                if (mp.x >= 0 && mp.x < (int)tileset.getSize().x &&
                    mp.y >= 0 && mp.y < (int)tileset.getSize().y)
                {
                    int tx = mp.x / tileSize;
                    int ty = mp.y / tileSize;
                    selectedRect = sf::IntRect(
                        tx * tileSize,
                        ty * tileSize,
                        tileSize,
                        tileSize
                    );
                }
            }

            // -------------------------------
            // Layer List
            // -------------------------------
            drawLayerList(window, layers, activeLayer, font, ev);

            // Update slider pointer if active layer changed
            if (activeLayer >= 0 && activeLayer < (int)layers.size())
                parallaxSlider.bindTo(&layers[activeLayer].paralax);

            if (activeLayer >= 0)
                parallaxSlider.handleEvent(ev, window);
        }

        // ============================================================================
        // Draw
        // ============================================================================
        window.clear(sf::Color(30, 30, 30));

        drawTileset(window, selectedRect);
        addBtn.draw(window);

        sf::Event dummy{};
        drawLayerList(window, layers, activeLayer, font, dummy);

        if (activeLayer >= 0)
            drawParallaxUI(window, layers[activeLayer], font, parallaxSlider, dummy);

        window.display();
    }

    return selectedRect;
}


// ============================================================================
// Tileset
// ============================================================================
void TilePicker::drawTileset(sf::RenderWindow& window, const sf::IntRect& selectedRect)
{
    sf::Sprite spr(tileset);
    window.draw(spr);

    int cols = tileset.getSize().x / tileSize;
    int rows = tileset.getSize().y / tileSize;

    std::vector<sf::Vertex> lines;
    lines.reserve((cols + rows + 2) * 2);

    for (int x = 0; x <= cols; x++) {
        lines.emplace_back(sf::Vector2f(x * tileSize, 0), sf::Color(150,150,150));
        lines.emplace_back(sf::Vector2f(x * tileSize, rows * tileSize), sf::Color(150,150,150));
    }
    for (int y = 0; y <= rows; y++) {
        lines.emplace_back(sf::Vector2f(0, y * tileSize), sf::Color(150,150,150));
        lines.emplace_back(sf::Vector2f(cols * tileSize, y * tileSize), sf::Color(150,150,150));
    }

    window.draw(lines.data(), lines.size(), sf::Lines);

    sf::RectangleShape sel({ float(tileSize), float(tileSize) });
    sel.setPosition(selectedRect.left, selectedRect.top);
    sel.setFillColor(sf::Color(0,0,0,0));
    sel.setOutlineColor(sf::Color::Yellow);
    sel.setOutlineThickness(2.f);
    window.draw(sel);
}


// ============================================================================
// Layer List
// ============================================================================
void TilePicker::drawLayerList(
    sf::RenderWindow& window,
    std::vector<LayerInfo>& layers,
    int& activeLayer,
    sf::Font& font,
    const sf::Event& ev
)
{
    float panelX = tileset.getSize().x + 10.f;
    float y = 10.f;

    // Add button
    UIButton addBtn({ panelX, y }, { 240, 24 }, "+ Add Layer", font);
    addBtn.draw(window);

    if (addBtn.isClicked(ev, window)) {
        LayerInfo n;
        n.name = "Layer " + std::to_string(layers.size());
        n.paralax = 1.0f;
        layers.push_back(n);
        if (activeLayer < 0) activeLayer = 0;
        return;
    }

    y += 40.f;

    // Layer list
    for (int i = 0; i < (int)layers.size(); i++)
    {
        bool selected = (i == activeLayer);

        sf::RectangleShape entry({ 240, 28 });
        entry.setPosition(panelX, y);
        entry.setFillColor(selected ? sf::Color(120,120,120) : sf::Color(60,60,60));
        window.draw(entry);

        sf::Text t(layers[i].name, font, 14);
        t.setPosition(panelX + 10, y + 5);
        window.draw(t);

        // Select click
        if (ev.type == sf::Event::MouseButtonReleased &&
            ev.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2i mp = sf::Mouse::getPosition(window);
            if (mp.x >= panelX && mp.x <= panelX + 240 &&
                mp.y >= y && mp.y <= y + 28)
                activeLayer = i;
        }

        // Up button
        UIButton up({ panelX + 195, y }, { 20, 12 }, "↑", font);
        up.draw(window);
        if (up.isClicked(ev, window) && i > 0) {
            std::swap(layers[i], layers[i - 1]);
            activeLayer = i - 1;
            return;
        }

        // Down button
        UIButton down({ panelX + 195, y + 14 }, { 20, 12 }, "↓", font);
        down.draw(window);
        if (down.isClicked(ev, window) && i < (int)layers.size() - 1) {
            std::swap(layers[i], layers[i + 1]);
            activeLayer = i + 1;
            return;
        }

        // Delete button
        UIButton del({ panelX + 220, y }, { 20, 24 }, "X", font);
        del.draw(window);
        if (del.isClicked(ev, window)) {
            layers.erase(layers.begin() + i);
            if (layers.empty()) activeLayer = -1;
            else if (activeLayer >= (int)layers.size()) activeLayer = layers.size() - 1;
            return;
        }

        y += 36;
    }
}



// ============================================================================
// Parallax UI using UISlider
// ============================================================================
void TilePicker::drawParallaxUI(
    sf::RenderWindow& window,
    LayerInfo& layer,
    sf::Font& font,
    UISlider& slider,
    const sf::Event& ev
){
    float panelX = 10.f;
    float y = tileset.getSize().y + 10.f;

    sf::Text label("Parallax:", font, 14);
    label.setPosition(panelX, y);
    window.draw(label);

    slider.draw(window);

    // Value text
    float rounded = float(int(layer.paralax * 100)) / 100.f;
    sf::Text v(std::to_string(rounded), font, 12);
    v.setPosition(10.f, y + 22);
    window.draw(v);
}