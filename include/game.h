#pragma once

#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Allies.h"
#include "Enemy.h"
#include "Dojo.h"
#include "Renderer.h"

class Game {
private:
    Renderer renderer;
    Map* currentMap;
    Dojo* playerDojo;
    Ally** allies;
    Enemy** enemies;
    int currentWave;
    int playerResources;
    bool gameRunning;
    bool gameWon;
    bool gameLost;
    int maxAllies;
    int maxEnemies;
    int allyCount;
    int enemyCount;

public:
    Game();
    ~Game();
    
    void initialize();
    void run();
    void handleEvents();
    void update(float deltaTime);
    void render();
    void spawnEnemyWave();
    void checkGameOver();
    void placeAlly(int type, int gridX, int gridY);
    void removeAlly(int gridX, int gridY);
    void addResources(int amount);
    bool canPlaceAlly(int gridX, int gridY);
    void cleanup();
};