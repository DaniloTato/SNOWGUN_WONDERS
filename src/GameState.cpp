#include "GameState.hpp"
#include "Bullet.hpp"
#include "GameCamera.hpp"
#include <SFML/Graphics.hpp>
#include "Constants.hpp"
#include "SFML/System/Vector2.hpp"

GameState::GameState()
    : activeCameras({})
    , activeWindows({new sf::RenderWindow(sf::VideoMode(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT), "SFML Window")})
    , checkpoint({-1,-1})
    , playerHealth(3)
    , selectedWeapon(Bullet::Type::Normal)
{}

GameState& GameState::getInstance() {
    static GameState instance;
    return instance;
}

void GameState::addCamera(GameCamera* camera) {
    if (std::find(activeCameras.begin(), activeCameras.end(), camera) == activeCameras.end()) {
        activeCameras.push_back(camera);
    }
}

void GameState::removeCamera(GameCamera* camera) {
    activeCameras.erase(std::remove(activeCameras.begin(), activeCameras.end(), camera), activeCameras.end());
}

const std::vector<GameCamera*>& GameState::getActiveCameras() const {
    return activeCameras;
}

GameCamera* GameState::getMainCamera() const {
    int mainCameraIndex = static_cast<int>(CameraList::MAIN);
    if(activeCameras.size() >= mainCameraIndex){
        return activeCameras[mainCameraIndex];
    }
    return nullptr;
}

sf::RenderWindow* GameState::getMainWindow() const {
    if(!activeWindows.empty()){
        return activeWindows.front();
    }
    return nullptr;
}

void GameState::clearCameras() {
    for(auto& i:activeCameras){
        GameObject::destroy(i);
    }
    activeCameras.clear();
}

const float GameState::dt(){
    return dtValue;
}

void GameState::updateDt(){
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed = now - lastFrameTime;
    lastFrameTime = now;
    dtValue = elapsed.count();
}

GameCamera* GameState::getUiCamera() const{
    int uiCameraIndex = static_cast<int>(CameraList::UI);
    if(activeCameras.size() >= uiCameraIndex){
        return activeCameras[uiCameraIndex];
    }
    return nullptr;
}

void GameState::createCamera(CameraList type){

    size_t index = static_cast<size_t>(type);
    
    if(activeCameras.size() < static_cast<int>(CameraList::COUNT)){
        activeCameras.resize(static_cast<int>(CameraList::COUNT));
    }

    if (activeCameras[index]) {
        return;
    }

    activeCameras[index] = new GameCamera();
}

int GameState::getCrystalAmount() const{
    return crystals;
}

void GameState::addToCrystalAmount(int amount){
    crystals += amount;
}

int GameState::getPlayerHealth() const{
    return playerHealth;
}
    
void GameState::changePlayerHealth(int amount){
    playerHealth += amount;
}

void GameState::changeWeaponSelection(){
    if(selectedWeapon == Bullet::Type::Normal){
        selectedWeapon = Bullet::Type::BubbleGun;
    } else{
        selectedWeapon = Bullet::Type::Normal;
    }
}

Bullet::Type GameState::getWeaponSelection(){
    return selectedWeapon;
}

void GameState::setCheckpoint(sf::Vector2f position){
    checkpoint = position;
}

const sf::Vector2f GameState::getCheckpoint() const{
    return checkpoint;
}

bool GameState::hasCheckpoint() const{
    return checkpoint != sf::Vector2f{-1,-1};
}