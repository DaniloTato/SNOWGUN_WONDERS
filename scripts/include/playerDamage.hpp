#pragma once

#include "GeneralContext.hpp"
#include "TangibleObject.hpp"

namespace script {

void playerDamage(TangibleObject &tangible, const GeneralContext &ctx,
                  float invincibilityDuration, sf::Vector2f knockSpeed);

namespace PlayerDamageFunctions {

bool isInvincible(TangibleObject &tangible);
bool isKnocked(TangibleObject &tangible);
void QuitKnockedState(TangibleObject &tangible);
void forceHit(TangibleObject &tangible, int damage, float invincibilityTime,
              float hitLockTime, sf::Vector2f knockSpeed);
} // namespace PlayerDamageFunctions

} // namespace script