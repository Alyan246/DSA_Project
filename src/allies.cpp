#include "allies.h"


Ally::Ally(int type, int damage, int range, float attackSpeed, GridPosition pos) 
    : type(type), damage(damage), range(range), attackSpeed(attackSpeed), position(pos), isActive(true), attackCooldown(0.0f)
    {
        pixelPos.x = pos.x * 40.0f + 20.0f; 
        pixelPos.y = pos.y * 40.0f + 20.0f;
    }

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
ArcherTower::ArcherTower(GridPosition pos) 
    : Ally(1, 5, 5, 0.5f, pos), arrowSpeed(2.0f), arrowCount(0),
      enemyQueue([](Enemy* a, Enemy* b){
          
        if(a->getType() != b->getType()) { //priority queue logic (stronger enemies first) 
            return a->getType() < b->getType(); //Higher type first
        }
          return a->getHealth() < b->getHealth(); //Higher health first
      })
{
    for(int i = 0; i < MAX_ARROWS; i++) arrows[i] = nullptr;
}

ArcherTower::~ArcherTower(){
    for(int i = 0; i <MAX_ARROWS; i++) delete arrows[i];
}

bool ArcherTower::isInRange(const Enemy* enemy) const{
    if(!enemy || !enemy->getIsActive()) return false;
    
    GridPosition enemyPos = enemy->getPosition();
    int dx = std::abs(position.x - enemyPos.x);
    int dy = std::abs(position.y - enemyPos.y);
    
    // 5 block radius in all directions
    return (dx <= 5 && dy <= 5);
}

void ArcherTower::updateEnemyQueue(Enemy** enemies, int enemyCount){
    
    while(!enemyQueue.empty()){
        enemyQueue.pop();
    }
    
    
    for(int i = 0; i < enemyCount; i++){
        if(enemies[i] && enemies[i]->getIsActive() && isInRange(enemies[i])){
            enemyQueue.push(enemies[i]);
        }
    }
}

// Remove dead or out-of-range enemies from queue
void ArcherTower::cleanupQueue(){
    //create a temporary queue to filter out invalid enemies
    std::priority_queue<Enemy*, std::vector<Enemy*>, 
        std::function<bool(Enemy*, Enemy*)>> tempQueue(
            [](Enemy* a, Enemy* b){
                if(a->getType() != b->getType()){
                    return a->getType() < b->getType();
                }
                return a->getHealth() < b->getHealth();
            });
    
    //copy valid enemies to temp queue
    while(!enemyQueue.empty()){
        Enemy* enemy = enemyQueue.top();
        enemyQueue.pop();
        
        if(enemy && enemy->getIsActive() && isInRange(enemy)){
            tempQueue.push(enemy);
        }
    }
    
    //restore the queue
    enemyQueue = std::move(tempQueue);
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
    
    updateEnemyQueue(enemies, enemyCount);
    
    
    cleanupQueue();
    //attack enemy through priority
    if(!enemyQueue.empty() && attackCooldown <= 0.0f){
        Enemy* target = enemyQueue.top();
        if(target && target->getIsActive() && isInRange(target)){
            attack(target);
            attackCooldown = 1.0f / attackSpeed;
        }else{
            enemyQueue.pop();
        }
    }
}
void ArcherTower::attack(Enemy* target){
    spawnArrow(target);
}
void ArcherTower::spawnArrow(Enemy* target){
    for(int i = 0; i < MAX_ARROWS; i++){
        if(!arrows[i] || !arrows[i]->isActive()){
            delete arrows[i];
            arrows[i] = new Arrow(pixelPos, target, damage, arrowSpeed);
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
Arrow::Arrow(sf::Vector2f startPos, Enemy* target, int damage, float speed) 
    : currentPosition(startPos), target(target), damage(damage), speed(speed), active(true) {}

Arrow::~Arrow() {}

sf::Vector2f Arrow::getTargetPosition() const {
    if(target && target->getIsActive()){
        return target->getPixelPosition();
    }
    return currentPosition;
}

void Arrow::update(float deltaTime){
    if (!active || !target || !target->getIsActive()) {
        active = false;
        return;
    }
    
    targetPosition = target->getPixelPosition();
    
    sf::Vector2f direction = targetPosition - currentPosition;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if(distance < 5.0f){ 
        checkHit();
        return;
    }
    
    //normalize direction for constant speed
    if(distance > 0){
        direction.x /= distance;
        direction.y /= distance;
    }
    
    float moveAmount = speed * deltaTime * 50.0f; 
    currentPosition.x += direction.x * moveAmount;
    currentPosition.y += direction.y * moveAmount;
    
    sf::Vector2f newDirection = targetPosition - currentPosition;
    float newDistance = std::sqrt(newDirection.x * newDirection.x + newDirection.y * newDirection.y);
    
    if(newDistance < 5.0f){
        checkHit();
    }
}

void Arrow::checkHit(){
    if (target && target->getIsActive()){
        target->takeDamage(damage);
    }
    active = false;
}

bool Arrow::isActive() const{ 
    return active; 
}