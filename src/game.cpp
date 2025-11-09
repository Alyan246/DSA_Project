#include "game.h"
#include <iostream>

Game::Game() :
                currentMap(nullptr), playerDojo(nullptr), allies(nullptr), enemies(nullptr),
                currentWave(0), playerResources(100), gameRunning(false), gameWon(false), gameLost(false),
                maxAllies(50), maxEnemies(100), allyCount(0),spawnTimer(0.0f), spawnInterval(2.0f), enemiesToSpawn(20), enemiesSpawned(0), enemyCount(0), windowheight(sf::VideoMode::getDesktopMode().size.y * 0.8f), windowwidth(sf::VideoMode::getDesktopMode().size.x * 0.8f),
                window(sf::VideoMode({sf::VideoMode::getDesktopMode().size.x * 0.8f,sf::VideoMode::getDesktopMode().size.y * 0.8f}),"DojoDefender")
               {
    allies = new Ally*[maxAllies];
    enemies = new Enemy*[maxEnemies];
    for(int i = 0; i < maxAllies; i++) allies[i] = nullptr;
    for(int i = 0; i < maxEnemies; i++) enemies[i] = nullptr;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    int posX = (desktop.getDesktopMode().size.x - windowwidth) / 2;
    int posY = (desktop.getDesktopMode().size.y - windowheight) / 2.5;

    window.setPosition({posX,posY});
}

Game::~Game() {
    cleanup();
}

void Game::initialize() {
    // Initialize font
    if (!font.openFromFile("assets/font.ttf")) {
        std::cout << "Failed to load font, using default" << std::endl;
    }
    
    // Initialize map and dojo
    currentMap = new Map();
    playerDojo = new Dojo(currentMap->getDojoPosition(), 100);
    initializeEnemyStack();
    
    gameRunning = true;
}

void Game::run() {
    sf::Clock clock;
    float deltaTime = 0.0f;
    window.setVerticalSyncEnabled(true);
    
    while(gameRunning && window.isOpen()) {
        deltaTime = clock.restart().asSeconds();
        handleEvents();
        update(deltaTime);
        render();
        checkGameOver();
    }
}

void Game::handleEvents() {
    // pollEvent() now returns std::optional<sf::Event>
    while (auto eventOpt = window.pollEvent()) {
        const sf::Event& event = *eventOpt;

        // Handle window close
        if (event.is<sf::Event::Closed>()) {
            gameRunning = false;
            window.close();
        }

        // Example: handle key press (Escape quits)
        else if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
            if (key->scancode == sf::Keyboard::Scancode::Escape) {
                gameRunning = false;
                window.close();
            }
        }

        else if(const auto* mousekey = event.getIf<sf::Event::MouseButtonPressed>()){
            if(mousekey->button == sf::Mouse::Button::Left){
                if(canPlaceAlly(mousekey->position.x/40,mousekey->position.y/40)){
                    placeAlly(0,mousekey->position.x/40,mousekey->position.y/40);
                    cout<<"Placed"<<endl;
                }
                else{
                    cout<<"Not placed"<<endl;
                }
            }
            else{
                if(canPlaceAlly(mousekey->position.x,mousekey->position.y)){
                    placeAlly(1,mousekey->position.x,mousekey->position.y);
                }
            }
        }

        // You can also handle mouse, resize, etc.
        else if (const auto* resized = event.getIf<sf::Event::Resized>()) {
            std::cout << "Window resized to " 
                      << resized->size.x << "x" << resized->size.y << "\n";
        }
    }
}


void Game::update(float deltaTime) {
    spawnFromStack(deltaTime);

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

    float cellWidth  = static_cast<float>(windowwidth) / currentMap->getWidth()  ;
    float cellHeight = static_cast<float>(windowheight) / currentMap->getHeight() ;
    cout<<"Cell Width: "<<cellWidth<<endl;

    sf::RectangleShape cell(sf::Vector2f(cellWidth, cellHeight));
    cell.setOutlineThickness(1);
    cell.setOutlineColor(sf::Color::Black);
    
    int width = currentMap->getWidth();
    int height = currentMap->getHeight();
    
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            sf::Vector2f position(x * cellWidth , y *cellHeight);
            cell.setPosition(position);
            
            if(currentMap->getCellType(x,y) == 1 ) {
                cell.setFillColor(sf::Color::Blue);
            } 
            else if(currentMap->getCellType(x,y) == 2 ) {
                cell.setFillColor(sf::Color(139,69,19));
            }
            else{
                cell.setFillColor(sf::Color(100,200,100));
            }
            window.draw(cell);
        }
    }
}

void Game::renderAlly(const Ally& ally) {
    sf::CircleShape shape(15);
    GridPosition pos = ally.getPosition();
    sf::Vector2f allypos(pos.x * 40.0f + 5.0f, pos.y * 40.0f + 5.0f);
    shape.setPosition(allypos);

    if(ally.getType() == 0) {
        shape.setFillColor(sf::Color::Blue);
    } else {
        shape.setFillColor(sf::Color::Red);
    }
    
    window.draw(shape);
}

void Game::renderEnemy(const Enemy& enemy) {
    sf::CircleShape shape(12);
    sf::Vector2f enemyPos = enemy.getPixelPosition();
    shape.setPosition(enemyPos);
    
    if(enemy.getType() == 0){
        shape.setFillColor(sf::Color::White);
    } else if(enemy.getType() == 1) {
        shape.setFillColor(sf::Color::Yellow);
    } else {
        shape.setFillColor(sf::Color::Black);
    }
    
    window.draw(shape);
}

void Game::renderDojo() {
    if (!playerDojo) return;
    
    sf::RectangleShape shape(sf::Vector2f(60, 60));
    GridPosition pos = playerDojo->getPosition();
    sf::Vector2f dojopos(pos.x , pos.y );
    shape.setPosition(dojopos);
    shape.setFillColor(sf::Color(0, 100, 0));
    window.draw(shape);
    
    // Health bar
    float healthPercent = playerDojo->getHealthPercentage();
    sf::RectangleShape healthBar(sf::Vector2f(60 * healthPercent, 5));
    sf::Vector2f healthBarpos(pos.x * 40.0f - 10.0f, pos.y * 40.0f - 15.0f);
    healthBar.setPosition(healthBarpos);
    healthBar.setFillColor(sf::Color::Green);
    window.draw(healthBar);
}

void Game::renderUI() {
    sf::Text text(font,"",20);
    text.setFillColor(sf::Color::White);
    text.setString("Resources: " + std::to_string(playerResources));
    sf::Vector2f textpos(10.0f , 10.0f);
    text.setPosition(textpos);
    window.draw(text);
    
    text.setString("Wave: " + std::to_string(currentWave));

    text.setPosition(sf::Vector2f(10.0f,40.0f));
    window.draw(text);
    
    text.setString("Dojo Health: " + std::to_string(playerDojo->getHealth()));
    sf::Vector2f stringpos(10.0f , 70.0f);
    text.setPosition(stringpos);
    window.draw(text);
    
    text.setString("Allies: " + std::to_string(allyCount) + " | Enemies: " + std::to_string(enemyCount));
    
    text.setPosition(sf::Vector2f(10.0f,100.0f));
    window.draw(text);
}

GridPosition Game::getRandomSpawnPosition() {
    // Use grid coordinates, not pixel coordinates!
    int gridWidth = currentMap->getWidth();   // Grid width (e.g., 20)
    int gridHeight = currentMap->getHeight(); // Grid height (e.g., 15)
    
    // Spawn on right side (same X), random Y from 3 fixed positions
    int spawnX = gridWidth - 1;  // Fixed X position (right side of grid)
    
    // Three fixed vertical spawn positions in grid coordinates
    int verticalOptions[3] = {
        gridHeight / 4,      // Upper (25% from top)
        gridHeight / 2,      // Middle 
        gridHeight * 3 / 4   // Lower (75% from top)
    };
    
    // Choose random Y position
    int randomIndex = rand() % 3;
    int spawnY = verticalOptions[randomIndex];
    
    // Ensure spawnY is within valid grid bounds
    if (spawnY < 2) spawnY = 2;
    if (spawnY >= gridHeight - 2) spawnY = gridHeight - 3;
    
    std::cout << "Spawning at grid position: (" << spawnX << "," << spawnY << ")" << std::endl;
    return GridPosition(spawnX, spawnY);
}

void Game::initializeEnemyStack(){

    enemyStack.push(new Jonin(getRandomSpawnPosition()));
    enemyStack.push(new Genin(getRandomSpawnPosition()));
    for (int i = 0; i < 3; i++) {
        enemyStack.push(new Jonin(getRandomSpawnPosition()));
        enemyStack.push(new Chunin(getRandomSpawnPosition()));
        enemyStack.push(new Chunin(getRandomSpawnPosition()));
        enemyStack.push(new Genin(getRandomSpawnPosition()));
        enemyStack.push(new Genin(getRandomSpawnPosition()));
        enemyStack.push(new Genin(getRandomSpawnPosition()));
    }
    cout << "Enemy stack initialized with " << enemyStack.size() << " enemies" << std::endl;
}

void Game::spawnFromStack(float deltaTime){
    if (enemiesSpawned >= enemiesToSpawn) return;
    
    spawnTimer += deltaTime;
    
    if (spawnTimer >= spawnInterval && !enemyStack.empty()) {
        if(enemyCount < maxEnemies){
            Enemy* newEnemy = enemyStack.top();
            enemyStack.pop();
            enemies[enemyCount] = newEnemy;
            enemyCount++;
            enemiesSpawned++;
            cout << "Spawned enemy from stack. Type: " << newEnemy->getType() 
                      << " Remaining in stack: " << enemyStack.size() << std::endl;
        }
        
        spawnTimer = 0.0f;
    }
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
    if (!currentMap) {
        std::cout << "❌ currentMap is null!\n";
        return false;
    }
    if (!currentMap->isValidPosition(gridX, gridY)) {
        std::cout << "❌ Invalid position: " << gridX << "," << gridY << "\n";
        return false;
    }
    if (currentMap->getCellType(gridX,gridY) != 0) {
        std::cout << "❌ Cell not free: " << gridX << "," << gridY << "\n";
        return false;
    }

    for (int i = 0; i < allyCount; i++) {
        if (allies[i] && allies[i]->getIsActive()) {
            GridPosition pos = allies[i]->getPosition();
            if (pos.x == gridX && pos.y == gridY) {
                std::cout << "❌ Cell already has ally: " << gridX << "," << gridY << "\n";
                return false;
            }
        }
    }

    std::cout << "✅ Can place ally at " << gridX << "," << gridY << "\n";
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