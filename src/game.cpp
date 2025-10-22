#include "game.h"
#include <iostream>

Game::Game() : window(sf::VideoMode(1024, 768), "Dojo Defender"), 
               currentMap(nullptr), playerDojo(nullptr), allies(nullptr), enemies(nullptr),
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
    // Initialize font
    if (!font.loadFromFile("assets/font.ttf")) {
        std::cout << "Failed to load font, using default" << std::endl;
    }
    
    // Initialize map and dojo
    currentMap = new Map();
    playerDojo = new Dojo(currentMap->getDojoPosition(), 100);
    
    gameRunning = true;
}

void Game::run() {
    sf::Clock clock;
    float deltaTime = 0.0f;
    
    while(gameRunning && window.isOpen()) {
        deltaTime = clock.restart().asSeconds();
        handleEvents();
        update(deltaTime);
        render();
        checkGameOver();
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            gameRunning = false;
            window.close();
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
    window.clear(sf::Color(50, 50, 50));
    
    renderMap();
    renderDojo();
    
    // Render allies
    for(int i = 0; i < allyCount; i++) {
        if(allies[i] && allies[i]->getIsActive()) {
            renderAlly(*allies[i]);
        }
    }
    
    // Render enemies
    for(int i = 0; i < enemyCount; i++) {
        if(enemies[i] && enemies[i]->getIsActive()) {
            renderEnemy(*enemies[i]);
        }
    }
    
    renderUI();
    window.display();
}

void Game::renderMap() {
    if (!currentMap) return;
    
    sf::RectangleShape cell(sf::Vector2f(40, 40));
    cell.setOutlineThickness(1);
    cell.setOutlineColor(sf::Color::White);
    
    int width = currentMap->getWidth();
    int height = currentMap->getHeight();
    
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            cell.setPosition(x * 40.0f, y * 40.0f);
            
            if(currentMap->isCellFree(x, y)) {
                cell.setFillColor(sf::Color(100, 100, 100));
            } else {
                cell.setFillColor(sf::Color(139, 69, 19));
            }
            
            window.draw(cell);
        }
    }
}

void Game::renderAlly(const Ally& ally) {
    sf::CircleShape shape(15);
    GridPosition pos = ally.getPosition();
    shape.setPosition(pos.x * 40.0f + 5.0f, pos.y * 40.0f + 5.0f);
    
    if(ally.getType() == 0) {
        shape.setFillColor(sf::Color::Blue);
    } else {
        shape.setFillColor(sf::Color::Red);
    }
    
    window.draw(shape);
}

void Game::renderEnemy(const Enemy& enemy) {
    sf::CircleShape shape(12);
    GridPosition pos = enemy.getPosition();
    shape.setPosition(pos.x * 40.0f + 8.0f, pos.y * 40.0f + 8.0f);
    
    if(enemy.getType() == 0) {
        shape.setFillColor(sf::Color::Cyan);
    } else if(enemy.getType() == 1) {
        shape.setFillColor(sf::Color::Magenta);
    } else {
        shape.setFillColor(sf::Color::Yellow);
    }
    
    window.draw(shape);
}

void Game::renderDojo() {
    if (!playerDojo) return;
    
    sf::RectangleShape shape(sf::Vector2f(60, 60));
    GridPosition pos = playerDojo->getPosition();
    shape.setPosition(pos.x * 40.0f - 10.0f, pos.y * 40.0f - 10.0f);
    shape.setFillColor(sf::Color(0, 100, 0));
    window.draw(shape);
    
    // Health bar
    float healthPercent = playerDojo->getHealthPercentage();
    sf::RectangleShape healthBar(sf::Vector2f(60 * healthPercent, 5));
    healthBar.setPosition(pos.x * 40.0f - 10.0f, pos.y * 40.0f - 15.0f);
    healthBar.setFillColor(sf::Color::Green);
    window.draw(healthBar);
}

void Game::renderUI() {
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    
    text.setString("Resources: " + std::to_string(playerResources));
    text.setPosition(10.0f, 10.0f);
    window.draw(text);
    
    text.setString("Wave: " + std::to_string(currentWave));
    text.setPosition(10.0f, 40.0f);
    window.draw(text);
    
    text.setString("Dojo Health: " + std::to_string(playerDojo->getHealth()));
    text.setPosition(10.0f, 70.0f);
    window.draw(text);
    
    text.setString("Allies: " + std::to_string(allyCount) + " | Enemies: " + std::to_string(enemyCount));
    text.setPosition(10.0f, 100.0f);
    window.draw(text);
}

void Game::spawnEnemyWave() {
    // Implementation for spawning enemy waves
}

void Game::checkGameOver() {
    if (playerDojo->isDojoDestroyed()) {
        gameLost = true;
        gameRunning = false;
    }
}

void Game::placeAlly(int type, int gridX, int gridY) {
    if (allyCount >= maxAllies) return;
    
    if (type == 0) { // Samurai
        allies[allyCount] = new Samurai(GridPosition(gridX, gridY));
    } else { // Archer Tower
        allies[allyCount] = new ArcherTower(GridPosition(gridX, gridY));
    }
    allyCount++;
}

void Game::removeAlly(int gridX, int gridY) {
    for (int i = 0; i < allyCount; i++) {
        if (allies[i] && allies[i]->getIsActive()) {
            GridPosition pos = allies[i]->getPosition();
            if (pos.x == gridX && pos.y == gridY) {
                delete allies[i];
                allies[i] = nullptr;
                // Shift array to remove gap
                for (int j = i; j < allyCount - 1; j++) {
                    allies[j] = allies[j + 1];
                }
                allies[allyCount - 1] = nullptr;
                allyCount--;
                break;
            }
        }
    }
}

void Game::addResources(int amount) {
    playerResources += amount;
}

bool Game::canPlaceAlly(int gridX, int gridY) {
    if (!currentMap) return false;
    if (!currentMap->isValidPosition(gridX, gridY)) return false;
    if (!currentMap->isCellFree(gridX, gridY)) return false;
    
    // Check if position is already occupied by another ally
    for (int i = 0; i < allyCount; i++) {
        if (allies[i] && allies[i]->getIsActive()) {
            GridPosition pos = allies[i]->getPosition();
            if (pos.x == gridX && pos.y == gridY) {
                return false;
            }
        }
    }
    
    return true;
}

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