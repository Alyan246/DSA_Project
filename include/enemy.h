#pragma once

#include <SFML/Graphics.hpp>
#include "map.h"

class Enemy{
protected:
    int type;
    int health;
    int maxHealth;
    int damage;
    double speed;
    bool isMoving;
    GridPosition currentPosition;
    GridPosition* path;
    int pathLength;
    int currentPathIndex;
    bool isActive;
    bool reachedDojo;
    double dTime;
    sf::Vector2f pixelPosition;

public:
    Enemy(int type, int health, int damage, double speed, GridPosition spawnPos);
    virtual ~Enemy();
    
    virtual void update(double deltaTime, const Map& map);
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
    sf::Vector2f getPixelPosition() const;
};

class Genin : public Enemy{                 
public:
    Genin(GridPosition spawnPos);
    ~Genin();
};

class Chunin : public Enemy{   
public:
    Chunin(GridPosition spawnPos);
    ~Chunin();
};

class Jonin : public Enemy{   
public:
    Jonin(GridPosition spawnPos);
    ~Jonin();
};