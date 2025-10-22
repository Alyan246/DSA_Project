#pragma once

#include <SFML/Graphics.hpp>
#include "map.h"

class Enemy{
protected:
    int type;
    int health;
    int maxHealth;
    int damage;
    float speed;
    GridPosition currentPosition;
    GridPosition* path;
    int pathLength;
    int currentPathIndex;
    bool isActive;
    bool reachedDojo;

public:
    Enemy(int type, int health, int damage, float speed, GridPosition spawnPos);
    virtual ~Enemy();
    
    virtual void update(float deltaTime, const Map& map);
    void takeDamage(int amount);
    void moveAlongPath(float deltaTime);
    void calculatePath(const Map& map);
    bool hasReachedDojo() const;
    bool getIsActive() const;
    GridPosition getPosition() const;
    int getType() const;
    int getHealth() const;
    int getMaxHealth() const;
    int getDamage() const;
};

class Genin : public Enemy{    //white niggas                
public:
    Genin(GridPosition spawnPos);
    ~Genin();
};

class Chunin : public Enemy{   //yellow niggas
public:
    Chunin(GridPosition spawnPos);
    ~Chunin();
};

class Jonin : public Enemy{    //black niggas
public:
    Jonin(GridPosition spawnPos);
    ~Jonin();
};