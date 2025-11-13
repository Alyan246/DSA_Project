#pragma once

#include <SFML/Graphics.hpp>
#include <queue>
#include <vector>
#include <functional>
#include "enemy.h"

#define MAX_ARROWS 20

class Ally {
protected:
    int type;
    int damage;
    int range;
    float attackSpeed;
    float attackCooldown;
    GridPosition position;
    sf::Vector2f pixelPos;
    bool isActive;

public:
    Ally(int type, int damage, int range, float attackSpeed, GridPosition pos);
    virtual ~Ally();
    
    virtual void update(float deltaTime, Enemy** enemies, int enemyCount) = 0;
    virtual void attack(Enemy* target) = 0;
    bool isInRange(const GridPosition& targetPos) const;
    int getType() const;
    GridPosition getPosition() const;
    bool getIsActive() const;
    sf::Vector2f getPixelPos()const{return pixelPos;}
    void setPosition(GridPosition newPos);
};

class Samurai : public Ally {
private:
    int health;
    int maxHealth;

public:
    Samurai(GridPosition pos);
    ~Samurai();
    
    void update(float deltaTime, Enemy** enemies, int enemyCount) override;
    void attack(Enemy* target) override;
    void takeDamage(int damage);
    int getHealth() const;
    int getMaxHealth() const;
};

class ArcherTower : public Ally{
private:
    class Arrow* arrows[MAX_ARROWS];
    float arrowSpeed;
    int arrowCount;

    //priority queue decleration
    std::priority_queue<Enemy*, std::vector<Enemy*>, 
        std::function<bool(Enemy*, Enemy*)>> enemyQueue;

public:
    ArcherTower(GridPosition pos);
    ~ArcherTower();
    
    void update(float deltaTime, Enemy** enemies, int enemyCount) override;
    void attack(Enemy* target) override;
    void spawnArrow(Enemy* target);
    class Arrow** getArrows();
    int getArrowCount() const;

    void updateEnemyQueue(Enemy** enemies, int enemyCount);
    bool isInRange(const Enemy* enemy) const;
    void cleanupQueue();
};

class Arrow {
private:
    sf::Vector2f startPosition;
    sf::Vector2f currentPosition;
    sf::Vector2f targetPosition;
    Enemy* target;
    int damage;
    float speed;
    bool active;

public:
    Arrow(sf::Vector2f startPos, Enemy* target, int damage, float speed);
    ~Arrow();
    
    void update(float deltaTime);
    void checkHit();
    bool isActive() const;
    sf::Vector2f getStartPosition() const{ return startPosition; }
    sf::Vector2f getCurrentPosition() const{ return currentPosition; }
    sf::Vector2f getTargetPosition() const;
    Enemy* getTarget() const{ return target; }
};