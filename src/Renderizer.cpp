#include "Renderizer.hpp"
#include "Constants.hpp"
#include "GameState.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/System/Vector2.hpp"

std::vector<RenderEntry> Renderizer::registry;

Renderizer::Renderizer(const RenderizerParameters &params)
    : window(params.window), texture(params.texture), rect(params.rect),
      color(sf::Color::White), assignedCamera(params.camera),
      layer(params.layer), paralax(params.parallax), show(true),
      showCountDown(0.f), hasCulling(true) {
  sprite.setTexture(texture);
  sprite.setTextureRect(rect);
}

Renderizer::~Renderizer() { unregisterPair(this); }

void Renderizer::registerPair(GameObject *obj, Renderizer *rend,
                              bool isRectShape) {
  registry.push_back({obj, rend, isRectShape});
}

void Renderizer::unregisterPair(Renderizer *rend) {
  registry.erase(std::remove_if(registry.begin(), registry.end(),
                                [&](const RenderEntry &e) {
                                  return e.renderizer == rend;
                                }),
                 registry.end());
}

void Renderizer::assignCamera(GameCamera *cam) { assignedCamera = cam; }

bool Renderizer::isVisible() const {

  float zoom = assignedCamera ? assignedCamera->getZoom() : 1.f;

  float x = sprite.getPosition().x;
  float y = sprite.getPosition().y;

  float w = static_cast<float>(rect.width) * zoom;
  float h = static_cast<float>(rect.height) * zoom;

  if (x + w < 0.f)
    return false;
  if (x > Constants::SCREEN_WIDTH)
    return false;
  if (y + h < 0.f)
    return false;
  if (y > Constants::SCREEN_HEIGHT)
    return false;

  return true;
}

void Renderizer::render(GameObject *obj) {

  sf::Vector2f position = obj->position + obj->offset;

  sprite.setColor(color);

  if (!assignedCamera) {
    sprite.setPosition(position);
    if (hasCulling && !isVisible())
      return;
    sprite.setScale(1.f, 1.f);
  } else {
    sf::Vector2f screenPos = assignedCamera->worldToScreen(position, paralax);
    sprite.setPosition(screenPos);
    if (hasCulling && !isVisible())
      return;
    sprite.setScale(assignedCamera->getZoom(), assignedCamera->getZoom());
  }

  window.draw(sprite);
}

void Renderizer::renderRectShape(GameObject *obj) {

  sf::Vector2f position = obj->position + obj->offset;

  sf::RectangleShape rectShape;
  rectShape.setFillColor(color);

  if (!assignedCamera) {
    rectShape.setPosition(position);
    rectShape.setSize(static_cast<sf::Vector2f>(rect.getSize()));
  } else {
    sf::Vector2f screenPos = assignedCamera->worldToScreen(position, paralax);
    rectShape.setPosition(screenPos);
    rectShape.setSize(static_cast<sf::Vector2f>(rect.getSize()) *
                      assignedCamera->getZoom());
  }

  window.draw(rectShape);
}

void Renderizer::setColor(sf::Color newColor) { color = newColor; }

float Renderizer::getLayer() const { return layer; }

void Renderizer::turnOffCulling() { hasCulling = false; }

void Renderizer::setRect(const sf::IntRect &newRect, int direction) {
  sf::IntRect directionRect = newRect;
  directionRect.left =
      newRect.left + directionRect.width * ((direction - 1) / -2);
  directionRect.width = newRect.width * direction;
  sprite.setTextureRect(directionRect);
}

void Renderizer::renderAll() {
  std::stable_sort(registry.begin(), registry.end(),
                   [](const RenderEntry &a, const RenderEntry &b) {
                     return a.renderizer->getLayer() > b.renderizer->getLayer();
                   });

  for (auto &entry : registry) {

    Renderizer *rend = entry.renderizer;
    GameObject *obj = entry.object;

    if (!rend->shouldIRender())
      continue;

    if (entry.isRectShape) {
      rend->renderRectShape(obj);
    } else {
      rend->render(obj);
    }
  }
}

const sf::IntRect &Renderizer::getRect() const { return rect; }

void Renderizer::toggleShowEvery(float time) {
  showCountDown -= GameState::getInstance().dt();
  if (showCountDown <= 0) {
    show = !show;
    showCountDown = time;
  }
}

void Renderizer::showSprite() { show = true; }

bool Renderizer::shouldIRender() { return show; }

void Renderizer::setLayer(float newLayer) { layer = newLayer; }

void Renderizer::toggleColorEvery(float time, const sf::Color &color1,
                                  const sf::Color &color2) {
  colorCountDown -= GameState::getInstance().dt();
  if (colorCountDown < 0) {
    if (color == color1) {
      color = color2;
    } else {
      color = color1;
    }
    colorCountDown = time;
  }
}

void Renderizer::hide() { show = false; }