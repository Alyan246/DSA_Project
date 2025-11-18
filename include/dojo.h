#pragma once

#include <SFML/Graphics.hpp>
#include "map.h"

class Dojo {
private:
    int health;
    int maxHealth;
    GridPosition dojoposition;
    bool isDestroyed;

public:
    Dojo(GridPosition pos, int maxHealth = 100);
    ~Dojo();
    
    void takeDamage(int amount);
    bool isDojoDestroyed() const;
    int getHealth() const;
    int getMaxHealth() const;
    GridPosition getPosition() const;
    float getHealthPercentage() const;
};