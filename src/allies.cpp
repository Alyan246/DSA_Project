#include "allies.h"
#include <cmath>

Ally::Ally(int type, int damage, int range, float attackSpeed, GridPosition pos) 
    : type(type), damage(damage), range(range), attackSpeed(attackSpeed), position(pos), isActive(true), attackCooldown(0.0f){}

Ally::~Ally() {}

bool Ally::isInRange(const GridPosition& targetPos) const{
    int dx = std::abs(position.x - targetPos.x);
    int dy = std::abs(position.y - targetPos.y);
    return (dx <= range && dy <= range);
}

int Ally::getType() const { 
    return type; 
}

GridPosition Ally::getPosition() const{ 
    return position; 
}

bool Ally::getIsActive() const{ 
    return isActive; 
}

void Ally::setPosition(GridPosition newPos){ 
    position = newPos; 
}

// Samurai implementation
Samurai::Samurai(GridPosition pos) : Ally(0, 10, 1, 1.0f, pos), health(100), maxHealth(100){}

Samurai::~Samurai() {}

void Samurai::update(float deltaTime, Enemy** enemies, int enemyCount){
    if (!isActive) return;
    
    attackCooldown -= deltaTime;
    
    // Find closest enemy in range
    Enemy* closestEnemy = nullptr;
    float closestDistance = range + 1;
    
    for (int i = 0; i< enemyCount; i++) {
        if (enemies[i] && enemies[i]->getIsActive()){
            if (isInRange(enemies[i]->getPosition())){
                int dx = std::abs(position.x - enemies[i]->getPosition().x);
                int dy = std::abs(position.y - enemies[i]->getPosition().y);
                float distance = std::sqrt(dx*dx + dy*dy);
                
                if(distance < closestDistance){
                    closestDistance = distance;
                    closestEnemy = enemies[i];
                }
            }
        }
    }
    
    if(closestEnemy && attackCooldown <= 0.0f){
        attack(closestEnemy);
        attackCooldown = 1.0f / attackSpeed;
    }
}

void Samurai::attack(Enemy* target) {
    if(target){
        target->takeDamage(damage);
    }
}

void Samurai::takeDamage(int damage){
    health -= damage;
    if(health <= 0){
        health = 0;
        isActive = false;
    }
}

int Samurai::getHealth() const{ 
    return health; 
}

int Samurai::getMaxHealth() const{ 
    return maxHealth; 
}

// ArcherTower implementation
ArcherTower::ArcherTower(GridPosition pos) : Ally(1, 5, 3, 0.5f, pos), arrowSpeed(2.0f), arrowCount(0){
    for(int i = 0; i < MAX_ARROWS; i++) arrows[i] = nullptr;
}

ArcherTower::~ArcherTower(){
    for(int i = 0; i <MAX_ARROWS; i++) delete arrows[i];
}

void ArcherTower::update(float deltaTime, Enemy** enemies, int enemyCount){
    if(!isActive) return;
    
    attackCooldown -= deltaTime;
    
    // Update arrows
    for(int i = 0; i < MAX_ARROWS; i++){
        if(arrows[i] && arrows[i]->isActive()){
            arrows[i]->update(deltaTime);
        }
    }  
    // Find strongest enemy in range (priority queue logic)
    Enemy* strongestEnemy = nullptr;
    int highestHealth = -1;
    
    for(int i = 0; i < enemyCount; i++){
        if(enemies[i] && enemies[i]->getIsActive()){
            if(isInRange(enemies[i]->getPosition())){
                if(enemies[i]->getHealth() > highestHealth){
                    highestHealth = enemies[i]->getHealth();
                    strongestEnemy = enemies[i];
                }
            }
        }
    }
    if (strongestEnemy && attackCooldown <= 0.0f) {
        attack(strongestEnemy);
        attackCooldown = 1.0f / attackSpeed;
    }
}
void ArcherTower::attack(Enemy* target) {
    spawnArrow(target);
}
void ArcherTower::spawnArrow(Enemy* target) {
    for (int i = 0; i < MAX_ARROWS; i++) {
        if (!arrows[i] || !arrows[i]->isActive()) {
            delete arrows[i];
            arrows[i] = new Arrow(position, target, damage, arrowSpeed);
            break;
        }
    }
}
Arrow** ArcherTower::getArrows(){ 
    return arrows; 
}

int ArcherTower::getArrowCount() const{ 
    return arrowCount; 
}

// Arrow implementation
Arrow::Arrow(GridPosition startPos, Enemy* target, int damage, float speed) 
    : currentPosition(startPos), target(target), damage(damage), speed(speed), active(true) {}

Arrow::~Arrow() {}

void Arrow::update(float deltaTime){
    if (!active || !target || !target->getIsActive()) {
        active = false;
        return;
    }
    
    // Move arrow toward target
    GridPosition targetPos = target->getPosition();
    float dx = targetPos.x - currentPosition.x;
    float dy = targetPos.y - currentPosition.y;
    float distance = std::sqrt(dx*dx + dy*dy);
    
    if(distance < 0.1f){
        checkHit();
        return;
    }
    
    dx /= distance;
    dy /= distance;
    
    currentPosition.x += dx * speed * deltaTime;
    currentPosition.y += dy * speed * deltaTime;
    
    // Check if reached target
    float newDx = targetPos.x - currentPosition.x;
    float newDy = targetPos.y - currentPosition.y;
    float newDistance = std::sqrt(newDx*newDx + newDy*newDy);
    
    if(newDistance < 0.3f){
        checkHit();
    }
}

void Arrow::checkHit() {
    if (target && target->getIsActive()){
        target->takeDamage(damage);
    }
    active = false;
}

bool Arrow::isActive() const{ 
    return active; 
}

GridPosition Arrow::getPosition() const{ 
    return currentPosition; 
}

void Arrow::setActive(bool active){ 
    this->active = active; 
}