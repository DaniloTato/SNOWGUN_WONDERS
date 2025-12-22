#include "PhysicsComponent.hpp"
#include "SFML/System/Vector2.hpp"

PhysicsComponent::PhysicsComponent() 
    : gravity(0.3f), friction({0.9f,0.9f}), hasXFriction(true), hasYFriction(false) {
    speeds.resize(static_cast<std::size_t>(SpeedType::COUNT));
}

void PhysicsComponent::updateX(sf::Vector2f& position) {
    for(sf::Vector2f& speed : speeds){
        if(hasXFriction){
            speed.x *= friction.x;
        }
        position.x += speed.x;
    }
}

void PhysicsComponent::updateY(sf::Vector2f& position) {
    for(int i = 0; i < speeds.size(); i++){

        if(hasYFriction){
            speeds[i].y *= friction.y;
        }

        if(i == static_cast<int>(SpeedType::MOVEMENT)){
            speeds[i].y += gravity;
        }
        position.y += speeds[i].y;
    }
}

void PhysicsComponent::turnOffXFriction(){
    hasXFriction = false;
}

const sf::Vector2f& PhysicsComponent::getSpeed(SpeedType type) const {
    return speeds[static_cast<int>(type)]; 
}

float PhysicsComponent::getSpdx(SpeedType type) const { 
    return speeds[static_cast<int>(type)].x; 
}

float PhysicsComponent::getSpdy(SpeedType type) const { 
    return speeds[static_cast<int>(type)].y; 
}

void PhysicsComponent::setSpeed(const sf::Vector2f& newSpeed, SpeedType type) { 
    speeds[static_cast<int>(type)] = newSpeed; 
}

void PhysicsComponent::setSpdx(float x, SpeedType type) { 
    speeds[static_cast<int>(type)].x = x; 
}

void PhysicsComponent::setSpdy(float y, SpeedType type) { 
    speeds[static_cast<int>(type)].y = y; 
}

void PhysicsComponent::turnOnYFriction(){
    hasYFriction = true;
}

void PhysicsComponent::turnOffYFriction(){
    hasYFriction = false;
}