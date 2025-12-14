#include "enemyPatrol.hpp"
#include "Bullet.hpp"
#include "BulletManager.hpp"

namespace script {

    void enemyPatrol(TangibleObject& tangible, const GeneralContext& ctx){

        tangible.physics.setSpdx(1 * tangible.direction);
        tangible.physics.updateX(tangible.position);
        if(tangible.collider.horizontalLevelCollision(tangible.position)){
            tangible.direction *= -1;
        }

        tangible.physics.updateY(tangible.position);
        if(tangible.collider.verticalLevelCollision(tangible.position)){
            tangible.physics.setSpdy(0.f);
        }

        Bullet* hit = BulletManager::getInstance().isCollidingWithBullet(tangible);

        if (hit) {
            //tangible.takeDamage(hit->getDamage());
            BulletManager::getInstance().queueDeletion(hit);
        }
    }
}