#pragma once

#include <cmath>
#include <SFML/Graphics.hpp>
#include "enemy.h"

class Ally {
protected:
    int type;
    int damage;
    int range;
    float attackSpeed;
    float attackCooldown;
    GridPosition position;
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

class ArcherTower : public Ally {
private:
    static const int MAX_ARROWS = 20;
    class Arrow* arrows[MAX_ARROWS];
    float arrowSpeed;
    int arrowCount;

public:
    ArcherTower(GridPosition pos);
    ~ArcherTower();
    
    void update(float deltaTime, Enemy** enemies, int enemyCount) override;
    void attack(Enemy* target) override;
    void spawnArrow(Enemy* target);
    class Arrow** getArrows();
    int getArrowCount() const;
};

class Arrow {
private:
    GridPosition currentPosition;
    GridPosition targetPosition;
    Enemy* target;
    int damage;
    float speed;
    bool active;

public:
    Arrow(GridPosition startPos, Enemy* target, int damage, float speed);
    ~Arrow();
    
    void update(float deltaTime);
    void checkHit();
    bool isActive() const;
    GridPosition getPosition() const;
    void setActive(bool active);
};