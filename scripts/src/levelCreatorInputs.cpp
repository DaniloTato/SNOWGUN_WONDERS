#include "levelCreatorInputs.hpp"
#include "EnemyManager.hpp"
#include "InputManager.hpp"
#include "LevelManager.hpp"
#include "TilePicker.hpp"
#include "Constants.hpp"
#include "GameState.hpp"
#include <iostream>

namespace script {

    namespace {
        struct StoredSelection{
            sf::IntRect selectedTileRect;
            std::string selectedEnemyId;
            PickerMode mode;
        };

        StoredSelection storedSelection;
    }

    void levelCreatorInputs(ScriptRunner &scriptRunner, const GeneralContext &ctx){

        InputManager& inputManager = InputManager::getInstance();
        LevelManager& levelManager = LevelManager::getInstance();

        sf::RenderWindow& window = ctx.window;

        if (inputManager.isJustPressed("tilePicker")){
            TilePicker picker(
                levelManager.getTilesheet(),
                Constants::TILE_SIZE
            );

            PickerSelection selection = picker.open(levelManager.layers, levelManager.activeLayer);
            storedSelection.selectedTileRect = selection.tileRect;
            storedSelection.selectedEnemyId = selection.enemyId;
            storedSelection.mode = selection.mode;
            levelManager.reloadAllLayers(window, GameState::getInstance().getMainCamera());
        }

        if (inputManager.isJustPressed("spawnEnemy")){
            EnemyManager::getInstance().queueCreateEnemy(
                storedSelection.selectedEnemyId,
                GameState::getInstance().getMainCamera()->screenToWorld(
                    {static_cast<float>(inputManager.getMousePosition().x), static_cast<float>(inputManager.getMousePosition().y)},
                    1.f
                )
            );
        }

        if(storedSelection.mode == PickerMode::Tiles){
            if(inputManager.isPressed("createTile")){
                sf::Vector2f mousePosToTilePos = GameState::getInstance().getMainCamera()->screenToWorld(
                    {static_cast<float>(inputManager.getMousePosition().x), static_cast<float>(inputManager.getMousePosition().y)},
                    levelManager.getLayerInfo(levelManager.activeLayer).paralax
                );

                sf::IntRect& selRect = storedSelection.selectedTileRect;
                const int tileSize = Constants::TILE_SIZE;
                int tilesWide = selRect.width / tileSize;
                int tilesHigh = selRect.height / tileSize;
                int baseTileX = static_cast<int>(mousePosToTilePos.x) / tileSize;
                int baseTileY = static_cast<int>(mousePosToTilePos.y) / tileSize;

                for (int y = 0; y < tilesHigh; y++) {
                    for (int x = 0; x < tilesWide; x++) {
                        sf::IntRect subRect(
                            selRect.left + x * tileSize,
                            selRect.top + y * tileSize,
                            tileSize,
                            tileSize
                        );

                        levelManager.queueCreateTile(
                            window,
                            GameState::getInstance().getMainCamera(),
                            levelManager.activeLayer,
                            baseTileX + x,
                            baseTileY + y,
                            subRect
                        );
                    }
                }
            }
        } else if(storedSelection.mode == PickerMode::Enemies){
            if(inputManager.isJustPressed("createTile")){
                sf::Vector2f mousePosToEnemyPos = GameState::getInstance().getMainCamera()->screenToWorld(
                    {static_cast<float>(inputManager.getMousePosition().x), static_cast<float>(inputManager.getMousePosition().y)},
                    1.f
                );
                sf::Vector2i EnemyPosInt = {
                    static_cast<int>(mousePosToEnemyPos.x),
                    static_cast<int>(mousePosToEnemyPos.y)
                };

                EnemyPosInt /= Constants::TILE_SIZE;
                EnemyPosInt *= Constants::TILE_SIZE;

                EnemyManager::getInstance().queueCreateEnemy(
                    storedSelection.selectedEnemyId,
                    {static_cast<float>(EnemyPosInt.x), static_cast<float>(EnemyPosInt.y)}
                );
            }
        }

        if(inputManager.isPressed("deleteTile")){
            sf::Vector2f mousePosToTilePos = GameState::getInstance().getMainCamera()->screenToWorld(
                {static_cast<float>(inputManager.getMousePosition().x), static_cast<float>(inputManager.getMousePosition().y)},
                levelManager.getLayerInfo(levelManager.activeLayer).paralax
            );

            levelManager.queueDeleteTile(
                levelManager.activeLayer,
                static_cast<int>(mousePosToTilePos.x) / Constants::TILE_SIZE,
                static_cast<int>(mousePosToTilePos.y) / Constants::TILE_SIZE
            );
        }

        if(inputManager.isJustPressed("saveLevel")){
            std::cout << "Saving level..." << std::endl;
            levelManager.saveLevel("assets/level_data/level.json");
        }
    }
}