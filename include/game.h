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
    sf::Texture boundarytree; 
    sf::Texture obstacletree; 
    sf::Texture grass;
    sf::Texture Soil;
    sf::Texture Dojotexture;
    sf::Texture Spawn;
    sf::Texture Arrowtexture;
    sf::Texture SamuraiRunning;
    sf::Texture SamuraiAttacking;
    sf::Texture SamuraiStanding;
    sf::Texture Whiterunning;
    sf::Texture Yellowrunning;
    sf::Texture BlackRunning;
    sf::Texture Archertowertex;
    sf::Texture YellowIdle;
    sf::Texture titleScreen;
    sf::Texture howToPlayScreen;
    sf::Texture BlackAttack , WhiteAttack , YellowAttack;
    int currentScreen;
    bool showTitleScreens;
    sf::Texture BlackIdle , WhiteIdle;

    float spawnTimer;              
    float spawnInterval;            
    int enemiesToSpawn;      
    int enemiesSpawned;
    int enemiesDefeated;
    int currentWave;
    int samuraiLeft;
    int archerTowersLeft;
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
    void checkGameOver();
    void placeAlly(int type, int gridX, int gridY);
    void removeAlly(int gridX, int gridY);
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

    void handleTitleScreenEvents();
    void renderTitleScreen();
    void loadTitleScreens();


};