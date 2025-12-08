#include <SFML/Graphics.hpp>
#include "GameObject.hpp"
#include "LevelManager.hpp"
#include "RenderableObject.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "TangibleObject.hpp"

int main() {

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");
    window.setFramerateLimit(60);

    sf::Texture playerTexture;
    playerTexture.loadFromFile("assets/snowman.png");

    sf::IntRect playerRect(0, 0, 17, 17);

    RenderizerParameters params{
        window,
        playerTexture,
        playerRect,
        {16.f, 16.f}
    };

    TangibleObject player(params);
    player.collider.setSize({16.f, 16.f});

    LevelManager::getInstance().loadLevel(window, "assets/level_data/level.txt");

    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed){
                window.close();
            }
        }

        window.clear();
        for (GameObject* gameObject : GameObject::getGameObjects()) {
            gameObject->update();
        }
        window.display();
    }
    
    return 0;
}