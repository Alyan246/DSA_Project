#include "Enemy.h"
#include <cmath>

Enemy::Enemy(int type, int health, int damage, float speed, GridPosition spawnPos) 
    : type(type), health(health), maxHealth(health), damage(damage), speed(speed), 
      currentPosition(spawnPos), path(nullptr), pathLength(0), currentPathIndex(0), 
      isActive(true), reachedDojo(false) {}

Enemy::~Enemy() {
    delete[] path;
}

void Enemy::update(float deltaTime, const Map& map) {
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

void Enemy::moveAlongPath(float deltaTime) {
    if (currentPathIndex >= pathLength) return;
    
    GridPosition targetPos = path[currentPathIndex];
    float dx = targetPos.x - currentPosition.x;
    float dy = targetPos.y - currentPosition.y;
    float distance = std::sqrt(dx*dx + dy*dy);
    
    if (distance < 0.1f) {
        currentPathIndex++;
        return;
    }
    
    dx /= distance;
    dy /= distance;
    
    currentPosition.x += dx * speed * deltaTime;
    currentPosition.y += dy * speed * deltaTime;
    
    // Check if reached waypoint
    float newDx = targetPos.x - currentPosition.x;
    float newDy = targetPos.y - currentPosition.y;
    float newDistance = std::sqrt(newDx*newDx + newDy*newDy);
    
    if (newDistance < 0.3f) {
        currentPosition = targetPos;
        currentPathIndex++;
    }
}

void Enemy::calculatePath(const Map& map) {
    // This is where A* pathfinding will be implemented
    // For now, create a simple straight path
    GridPosition dojoPos = map.getDojoPosition();
    
    // Simple path: move toward dojo in straight lines
    pathLength = 2;
    path = new GridPosition[pathLength];
    path[0] = GridPosition(dojoPos.x, currentPosition.y); // Move horizontally first
    path[1] = dojoPos; // Then move vertically
    currentPathIndex = 0;
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

// Genin implementation
Genin::Genin(GridPosition spawnPos) : Enemy(0, 50, 5, 1.0f, spawnPos) {}
Genin::~Genin() {}

// Chunin implementation
Chunin::Chunin(GridPosition spawnPos) : Enemy(1, 100, 10, 0.8f, spawnPos) {}
Chunin::~Chunin() {}

// Jonin implementation
Jonin::Jonin(GridPosition spawnPos) : Enemy(2, 200, 20, 0.6f, spawnPos) {}
Jonin::~Jonin() {}