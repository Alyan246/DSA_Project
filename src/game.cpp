#include "Game.h"
#include <iostream>

Game::Game() : currentMap(nullptr), playerDojo(nullptr), allies(nullptr), enemies(nullptr),
               currentWave(0), playerResources(100), gameRunning(false), gameWon(false), gameLost(false),
               maxAllies(50), maxEnemies(100), allyCount(0), enemyCount(0) {
    allies = new Ally*[maxAllies];
    enemies = new Enemy*[maxEnemies];
    for(int i = 0; i < maxAllies; i++) allies[i] = nullptr;
    for(int i = 0; i < maxEnemies; i++) enemies[i] = nullptr;
}

Game::~Game() {
    cleanup();
}

void Game::initialize() {
    gameRunning = true;
    // Map selection and initialization will be added later
}

void Game::run() {
    sf::Clock clock;
    float deltaTime = 0.0f;
    
    while(gameRunning && renderer.isWindowOpen()) {
        deltaTime = clock.restart().asSeconds();
        handleEvents();
        update(deltaTime);
        render();
        checkGameOver();
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (renderer.getWindow().pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            gameRunning = false;
            renderer.closeWindow();
        }
        // Add more event handling for tower placement, etc.
    }
}

void Game::update(float deltaTime) {
    // Update enemies
    for(int i = 0; i < enemyCount; i++) {
        if(enemies[i] && enemies[i]->getIsActive()) {
            enemies[i]->update(deltaTime, *currentMap);
        }
    }
    
    // Update allies
    for(int i = 0; i < allyCount; i++) {
        if(allies[i] && allies[i]->getIsActive()) {
            allies[i]->update(deltaTime, enemies, enemyCount);
        }
    }
}

void Game::render() {
    renderer.beginFrame();
    renderer.renderMap(*currentMap);
    renderer.renderDojo(*playerDojo);
    
    // Render allies
    for(int i = 0; i < allyCount; i++) {
        if(allies[i] && allies[i]->getIsActive()) {
            renderer.renderAlly(*allies[i]);
        }
    }
    
    // Render enemies
    for(int i = 0; i < enemyCount; i++) {
        if(enemies[i] && enemies[i]->getIsActive()) {
            renderer.renderEnemy(*enemies[i]);
        }
    }
    
    renderer.renderUI(playerResources, currentWave, playerDojo->getHealth());
    renderer.endFrame();
}

void Game::spawnEnemyWave() {}
void Game::checkGameOver() {}
void Game::placeAlly(int type, int gridX, int gridY) {}
void Game::removeAlly(int gridX, int gridY) {}
void Game::addResources(int amount) {}
bool Game::canPlaceAlly(int gridX, int gridY) { return false; }

void Game::cleanup() {
    delete currentMap;
    delete playerDojo;
    if(allies) {
        for(int i = 0; i < maxAllies; i++) delete allies[i];
        delete[] allies;
    }
    if(enemies) {
        for(int i = 0; i < maxEnemies; i++) delete enemies[i];
        delete[] enemies;
    }
}