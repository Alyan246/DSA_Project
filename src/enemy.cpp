#include "enemy.h"
#include <cmath>

Enemy::Enemy(int type, int health, int damage, double speed, GridPosition spawnPos) 
    : type(type), health(health), maxHealth(health), damage(damage), speed(speed), 
      currentPosition(spawnPos), path(nullptr), isMoving(true), pathLength(0), currentPathIndex(0), 
      isActive(true), reachedDojo(false) {
        sf::Clock deltaClock;
        double dTime = deltaClock.restart().asSeconds(); 

        pixelPosition.x = spawnPos.x * 40.6;  
        pixelPosition.y = spawnPos.y * 42.2;
      }

Enemy::~Enemy() {
    delete[] path;
}

void Enemy::update(double deltaTime, Map * map , Ally *allies , int count) {
    if (!isActive) return;
    
    if (path == nullptr || currentPathIndex >= pathLength) {
        path = new vector<GridPosition>(map->getoptimumpath(allies,count));
        pathLength = path->size();
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
void Enemy::setPath(vector<GridPosition> enemypath) {
    path = new vector<GridPosition>( enemypath);
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