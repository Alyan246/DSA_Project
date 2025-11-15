#include "enemy.h"
#include <cmath>

Enemy::Enemy(int type, int health, int damage, double speed, GridPosition spawnPos , GridPosition dojo) 
    : type(type), health(health), maxHealth(health), damage(damage), speed(speed), 
      currentPosition(spawnPos), path(nullptr), isMoving(true), pathLength(0), currentPathIndex(0), 
      isActive(true), reachedDojo(false) , spawnposition(spawnPos) , dojopos(dojo) {
        sf::Clock deltaClock ;
        double dTime = deltaClock.restart().asSeconds(); 

        pixelPosition.x = spawnPos.x * 40;  
        pixelPosition.y = spawnPos.y * 40;
      }

Enemy::~Enemy() {
    delete[] path;
}

void Enemy::update(double deltaTime, Map * map , Ally *allies , int count) {
    if (!isActive) return;
    
    // if (path == nullptr ) {
    //     path = new vector<GridPosition>(map->getoptimumpath(allies,count));
    //     pathLength = path->size();
    //     cout<<"Path calculated and set"<<endl<<endl<<endl<<endl;
    // }
    
    // path = new vector<GridPosition>(map->getoptimumpath(allies,count));
    // pathLength = path->size();
    moveAlongPath(deltaTime, map->getgrid());
}


void Enemy::takeDamage(int amount) {
    health -= amount;
    if (health <= 0) {
        health = 0;
        isActive = false;
    }
}

void Enemy::moveAlongPath(float deltaTime, vector<vector<int>> grid){
   
    if(!isMoving){ cout<<"Not moving"<<endl ; return ; }

    float moveAmount = speed * deltaTime * 2.0f;
    
    pathLength = path->size();
    for(int i = 0 ; i< pathLength ; i++){
        // cout<<"Inside"<<endl;
        GridPosition target = path->at(i);
        // cout<<"at "<<i<<" "<<target.x<<" ,"<<target.y<<endl;
    }
    
    if(!path->empty() && currentPathIndex < pathLength){
        // cout<<"PATH IS NOT EMPTY"<<endl;
        GridPosition target = path->at(currentPathIndex);
        int TargetPixelX = target.x * 40;
        int TargetPixelY = target.y * 40;

        float dx = TargetPixelX - pixelPosition.x;
        float dy = TargetPixelY - pixelPosition.y;

        bool reachedX = fabs(dx) <= moveAmount;
        bool reachedY = fabs(dy) <= moveAmount;

        

        if (!reachedX)
            pixelPosition.x += (dx > 0 ? moveAmount : -moveAmount);
        else
            pixelPosition.x = TargetPixelX;

        if (!reachedY)
            pixelPosition.y += (dy > 0 ? moveAmount : -moveAmount);
        else
            pixelPosition.y = TargetPixelY;

        if (reachedX && reachedY)
            currentPathIndex++;


        currentPosition.x = round(pixelPosition.x /40);
        currentPosition.y = round(pixelPosition.y /40);
        
    }


    // stop at target (left side for now)
    float xdojo = dojopos.x * 40;
    float ydojo = dojopos.y *40;
    if(pixelPosition.x  - dojopos.x <= 1.0f && pixelPosition.y - dojopos.y <= 1.0f ){

        isMoving = false;
    }
}

//commented out for future use in recursive pathfinding algorithm
void Enemy::setPath(vector<GridPosition> enemypath) {
    path = new vector<GridPosition>( enemypath);
    pathLength = path->size();
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
Genin::Genin(GridPosition spawnPos , GridPosition dojo) : Enemy(0, 50, 5 , 30.0f, spawnPos , dojo) {}
Genin::~Genin() {}

// Chunin implementation
Chunin::Chunin(GridPosition spawnPos , GridPosition dojo) : Enemy(1, 100, 10, 25.0f, spawnPos , dojo) {}
Chunin::~Chunin() {}

// Jonin implementation
Jonin::Jonin(GridPosition spawnPos , GridPosition dojo) : Enemy(2, 200, 20, 20.0f, spawnPos , dojo) {}
Jonin::~Jonin() {}