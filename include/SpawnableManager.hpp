#pragma once
#include <unordered_map>
#include <string>
#include <functional>
#include "QueuedManager.hpp"
#include <SFML/Graphics.hpp>

template<typename TObject, typename TRequest>
class SpawnableManager : public QueuedManager<TObject, TRequest> {
protected:
    using FactoryFn = std::function<TObject*(const sf::Vector2f&)>;

    std::unordered_map<std::string, FactoryFn> templates;

public:
    void registerTemplate(const std::string& name, FactoryFn fn) {
        templates[name] = std::move(fn);
    }

    TObject* createFromRequest(const TRequest& req) override {
        return createFromTemplate(req.templateName, req.position);
    }

protected:
    TObject* createFromTemplate(
        const std::string& name,
        const sf::Vector2f& pos
    ) {
        auto it = templates.find(name);
        if (it == templates.end()) return nullptr;
        return it->second(pos);
    }
};