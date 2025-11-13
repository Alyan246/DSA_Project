#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>
#include <random>
#include <stack>
#include <ctime>
#include <cstdlib>
#include "map.h"
#include "allies.h"
#include "enemy.h"
#include "dojo.h"
using namespace std;

class Game {
private:
    sf::RenderWindow window;
    sf::Font font;
    GridPosition enemyspawnpoint;
    Map* currentMap;
    Dojo* playerDojo;
    Ally** allies;
    Enemy** enemies;
    std::stack<Enemy*> enemyStack;  
    float spawnTimer;              
    float spawnInterval;            
    int enemiesToSpawn;      
    int enemiesSpawned;
    int currentWave;
    int playerResources;
    bool gameRunning;
    bool gameWon;
    bool gameLost;
    int maxAllies;
    int maxEnemies;
    int allyCount;
    int enemyCount;
    int windowheight;
    int windowwidth;
    
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
    void initializeEnemyStack();  
    void spawnFromStack(float deltaTime);
    bool canPlaceAlly(int gridX, int gridY);
    void cleanup();
    GridPosition getenemyspawnpoint() const;
    void renderEnemyspawn();

    // Rendering methods
    void renderMap();
    void renderAlly(const Ally& ally);
    void renderEnemy(const Enemy& enemy);
    void renderDojo();
    void renderUI();
    void renderArrows();
};