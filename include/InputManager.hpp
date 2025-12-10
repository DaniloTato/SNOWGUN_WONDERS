#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <unordered_map>
#include <string>

enum class MouseButton {
    Left,
    Right,
    Middle
};

class InputManager {
public:
    static InputManager& getInstance() {
        static InputManager instance;
        return instance;
    }

    void handleEvent(const sf::Event& event);
    void update();  

    bool isPressed(const std::string& action) const;
    bool isJustPressed(const std::string& action) const;
    bool isJustReleased(const std::string& action) const;

    bool loadBindingsFromJsonFile(const std::string& filePath);
    void bindKey(const std::string& action, sf::Keyboard::Key key);

    bool isMousePressed(MouseButton button) const;
    bool isMouseJustPressed(MouseButton button) const;
    bool isMouseJustReleased(MouseButton button) const;

    sf::Vector2i getMousePosition() const;

    sf::Keyboard::Key keyFromString(const std::string& keyName) const;

private:
    InputManager();

    std::unordered_map<sf::Keyboard::Key, bool> currentState;
    std::unordered_map<sf::Keyboard::Key, bool> previousState;

    std::unordered_map<MouseButton, bool> mouseCurrent;
    std::unordered_map<MouseButton, bool> mousePrevious;

    sf::Vector2i lastMouseClickPosition;

    std::unordered_map<std::string, sf::Keyboard::Key> bindings;

    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

    InputManager(InputManager&&) = delete;
    InputManager& operator=(InputManager&&) = delete;
};