#include "TangibleObject.hpp"
#include "GeneralContext.hpp"
#include "SoundManager.hpp"
#include <SFML/Audio.hpp>

TangibleObject::TangibleObject(RenderizerParameters params, Animations cachedAnimations) 
    : GameObject(params.position)
    , renderizer(params) {
    animator.setAnimations(cachedAnimations);
    Renderizer::registerPair(this, &renderizer, params.registerAsRectShape);
}

void TangibleObject::update(const GeneralContext& ctx) {
    collider.computeCollisionGrid(position);
    scripter.runScripts(*this, ctx);
    animator.update();
    renderizer.setRect(animator.getCurrentFrame(), direction);
}

void TangibleObject::playSound(const std::string& id, float volume) {
    sf::Sound& s = sounds[id];
    s.setBuffer(SoundManager::getInstance().get(id));
    s.setVolume(volume);
    s.play();
}

bool TangibleObject::isPlayingAnySound() const {
    for (const auto& [id, sound] : sounds) {
        if (sound.getStatus() == sf::Sound::Playing &&
            sound.getVolume() > 0.f) {
            return true;
        }
    }
    return false;
}