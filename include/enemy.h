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
    vector<GridPosition>* path;
    int pathLength;
    int currentPathIndex;
    bool isActive;
    bool reachedDojo;
    double dTime;
    sf::Vector2f pixelPosition;

public:
    Enemy(int type, int health, int damage, double speed, GridPosition spawnPos);
    virtual ~Enemy();
    
    virtual void update(double deltaTime, Map * map , Ally *allies , int count);
    void takeDamage(int amount);
    void moveAlongPath(float deltaTime);
    void setPath(vector<GridPosition> path);
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