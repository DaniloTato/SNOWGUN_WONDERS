#include "BasicCollider.hpp"
#include "Constants.hpp"
#include "SFML/System/Vector2.hpp"
#include "LevelManager.hpp"

#include <vector>

//may want to rewrite some of this

void BasicCollider::setSize(sf::Vector2f newSize){
    size = newSize;
}

void BasicCollider::calculateCollisionGrid(sf::Vector2f position) {
    collisionGrid.clear();

    const int TILE_SIZE = Constants::TILE_SIZE;
    const int COLLISION_GRID_WIDTH = Constants::COLLISION_GRID_WIDTH;
    const int COLLISION_GRID_HEIGHT = Constants::COLLISION_GRID_HEIGHT;

    for (int j = 0; j < COLLISION_GRID_WIDTH * COLLISION_GRID_HEIGHT; j++){
        int square_x = ((int) position.x / TILE_SIZE) * TILE_SIZE + TILE_SIZE * j - COLLISION_GRID_WIDTH * TILE_SIZE * ((int) j / COLLISION_GRID_WIDTH) - TILE_SIZE;
        int square_y = ((int) position.y / TILE_SIZE) * TILE_SIZE + TILE_SIZE * ((int) j / COLLISION_GRID_WIDTH) - TILE_SIZE;
        collisionGrid.push_back(square_x / TILE_SIZE);
        collisionGrid.push_back(square_y / TILE_SIZE);
    }
}

bool BasicCollider::isColliding(sf::Vector2f position1, sf::Vector2f size1, sf::Vector2f position2, sf::Vector2f size2){
    bool horizontal = (position1.x + size1.x > position2.x && position1.x < position2.x + size2.x);
    bool vertical = (position1.y + size1.y > position2.y && position1.y < position2.y + size2.y);
    return (horizontal && vertical);
}

bool BasicCollider::isCollidingWithLevel(sf::Vector2f position, int i){
    const int TILE_SIZE = Constants::TILE_SIZE;
    return isColliding(position, size, sf::Vector2f(collisionGrid[i * 2] * TILE_SIZE, collisionGrid[i * 2 + 1] * TILE_SIZE), sf::Vector2f(TILE_SIZE, TILE_SIZE));
}

bool BasicCollider::horizontalLevelCollision(sf::Vector2f& position){

    const std::vector<std::vector<int>>& levelLayout = LevelManager::getInstance().getLevelLayout();
    const int TILE_SIZE = Constants::TILE_SIZE;

    bool didCollisionHappen = false;
    
    int movementDirection = -1 + (positionLastFrame.x < position.x)*2;

    for (int i = 0; i < (collisionGrid.size() * 0.5); i++){
        if(collisionGrid[i * 2 + 1] < levelLayout.size()){
            if (collisionGrid [i * 2] < levelLayout[collisionGrid[i * 2 + 1]].size()){

                if(levelLayout[collisionGrid[i * 2 + 1]][collisionGrid[i * 2]] != 0 && isCollidingWithLevel(position, i)){
                    directionLastCollision = movementDirection;
                    didCollisionHappen = true;
                    positionLastFrame.x = position.x;
                    //spdx = 0;
                    if(movementDirection > 0){
                        position.x -= (position.x + size.x) - collisionGrid[i * 2] * TILE_SIZE;
                    } else{
                        position.x += (collisionGrid[i * 2] * TILE_SIZE + TILE_SIZE) - position.x;
                    }
                }
            }
        }
    }

    positionLastFrame.x = position.x;
    
    return didCollisionHappen;
}

bool BasicCollider::verticalLevelCollision(sf::Vector2f& position){

    const std::vector<std::vector<int>>& levelLayout = LevelManager::getInstance().getLevelLayout();
    const int TILE_SIZE = Constants::TILE_SIZE;
    bool didCollisionHappen = false;
    //_time_on_air++;

    int movementDirection = -1 + (positionLastFrame.y < position.y)*2;

    for (int i = 0; i < (collisionGrid.size() * 0.5); i++){
        if(collisionGrid[i * 2 + 1] < levelLayout.size()){
            if (collisionGrid [i * 2] < levelLayout[collisionGrid[i * 2 + 1]].size()){

                if(levelLayout[collisionGrid[i * 2 + 1]][collisionGrid[i * 2]] != 0 && isCollidingWithLevel(position, i)){
                    didCollisionHappen = true;
                    //spdy = 0;
                    positionLastFrame.y = position.y;
                    if(movementDirection > 0){
                        position.y -= (position.y + size.y) - collisionGrid[i * 2 + 1] * TILE_SIZE;
                        //_time_on_air = 0;
                    } else{
                        position.y += (collisionGrid[i * 2 + 1] * TILE_SIZE + TILE_SIZE) - position.y;
                    }
                }
            }
        }
    }

    positionLastFrame.y = position.y;

    return didCollisionHappen;
}