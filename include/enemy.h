#pragma once

#include <SFML/Graphics.hpp>
#include "map.h"
#include "dojo.h"

class Enemy{
protected:
    int type;
    int health;
    int maxHealth;
    Ally* currentTarget;
    bool isAttacking;
    float attackCooldown;
    float attackTimer;
    int damage;
    double speed;
    bool isMoving;
    GridPosition currentPosition , spawnposition;
    vector<GridPosition>* path;
    float animationTimer;
    int currentAnimFrame;
    int pathLength;
    int currentPathIndex;
    bool isActive;
    bool reachedDojo;
    GridPosition dojopos;
    double dTime;
    sf::Vector2f pixelPosition;

public:
    Enemy(int type, int health, int damage, double speed, GridPosition spawnPos , GridPosition dojo);
    virtual ~Enemy();
    virtual void update(double deltaTime, Map * map , Ally** allies ,Dojo* dojo, int count);
    void takeDamage(int amount);
    void moveAlongPath(float deltaTime, vector<vector<int>> grid);
    void setPath(vector<GridPosition> path);
    bool hasReachedDojo() const;
    int getAnimationFrame() const;
    bool getIsActive() const;
    GridPosition getPosition() const;
    bool getisAttacking() const;
    int getType() const;
    int getHealth() const;
    int getMaxHealth() const;
    int getDamage() const;
    bool getismoving() const;
    //methods for attacking
    void checkForTargets(Ally** allies, int allyCount, Dojo* dojo);
    void attackTarget(float deltaTime, Dojo* dojo);
    bool isSamuraiInRange(Ally* samurai) const;
    
    sf::Vector2f getPixelPosition() const;
};

class Genin : public Enemy{                 
public:
    Genin(GridPosition spawnPos , GridPosition dojo);
    ~Genin();
};

class Chunin : public Enemy{   
public:
    Chunin(GridPosition spawnPos , GridPosition dojo);
    ~Chunin(); 
};

class Jonin : public Enemy{   
public:
    Jonin(GridPosition spawnPos , GridPosition dojo);
    ~Jonin();
};