#include "enemy.h"
#include <cmath>

Enemy::Enemy(int type, int health, int damage, double speed, GridPosition spawnPos) 
    : type(type), health(health), maxHealth(health), damage(damage), speed(speed), 
      currentPosition(spawnPos), path(nullptr), isMoving(true), pathLength(0), currentPathIndex(0), 
      isActive(true), reachedDojo(false) {
        sf::Clock deltaClock;
        double dTime = deltaClock.restart().asSeconds(); 

        pixelPosition.x = spawnPos.x * 40.0f + 20.0f;  
        pixelPosition.y = spawnPos.y * 40.0f + 20.0f;
      }

Enemy::~Enemy() {
    delete[] path;
}

void Enemy::update(double deltaTime, const Map& map) {
    if (!isActive) return;
    
    if (path == nullptr || currentPathIndex >= pathLength) {
        calculatePath(map);
    }
    
    if (path != nullptr && currentPathIndex < pathLength) {
        moveAlongPath(deltaTime);
    }
}

void Enemy::takeDamage(int amount) {
    health -= amount;
    if (health <= 0) {
        health = 0;
        isActive = false;
    }
}

void Enemy::moveAlongPath(float deltaTime){
    if(!isMoving) return;

    float moveAmount = speed * deltaTime * 2.0f;
    pixelPosition.x -= moveAmount;
    
    //for traversing cells through pixels (according to game logic)
    currentPosition.x = static_cast<int>(pixelPosition.x / 40.0f);
    currentPosition.y = static_cast<int>(pixelPosition.y / 40.0f);
    
    // stop at target (left side for now)
    if(pixelPosition.x <= 50.0f){
        pixelPosition.x = 50.0f;
        isMoving = false;
    }
}

//commented out for future use in recursive pathfinding algorithm
void Enemy::calculatePath(const Map& map) {
    // std::cout << "CALCULATE PATH CALLED! Current position: (" 
    //           << currentPosition.x << "," << currentPosition.y << ")" << std::endl;
    // GridPosition dojoPos = map.getDojoPosition();
    
    // // Create gradual path
    // pathLength = 6;  // More gradual waypoints
    // path = new GridPosition[pathLength];
    
    // // Gradual movement toward dojo
    // path[0] = GridPosition(currentPosition.x - 5, currentPosition.y);
    // path[1] = GridPosition(currentPosition.x - 10, currentPosition.y);
    // path[2] = GridPosition(currentPosition.x - 15, currentPosition.y);
    // path[3] = GridPosition(dojoPos.x - 10, currentPosition.y);
    // path[4] = GridPosition(dojoPos.x - 5, dojoPos.y);
    // path[5] = dojoPos;
    
    // currentPathIndex = 0;
}

bool Enemy::hasReachedDojo() const { 
    return reachedDojo; 
}

bool Enemy::getIsActive() const { 
    return isActive; 
}

GridPosition Enemy::getPosition() const { 
    return currentPosition; 
}

int Enemy::getType() const { 
    return type; 
}

int Enemy::getHealth() const { 
    return health; 
}

int Enemy::getMaxHealth() const { 
    return maxHealth; 
}

int Enemy::getDamage() const { 
    return damage; 
}

sf::Vector2f Enemy::getPixelPosition() const{ 
    return pixelPosition; 
}

// Genin implementation
Genin::Genin(GridPosition spawnPos) : Enemy(0, 50, 5 , 30.0f, spawnPos) {}
Genin::~Genin() {}

// Chunin implementation
Chunin::Chunin(GridPosition spawnPos) : Enemy(1, 100, 10, 25.0f, spawnPos) {}
Chunin::~Chunin() {}

// Jonin implementation
Jonin::Jonin(GridPosition spawnPos) : Enemy(2, 200, 20, 20.0f, spawnPos) {}
Jonin::~Jonin() {}