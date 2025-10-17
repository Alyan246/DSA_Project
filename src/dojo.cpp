#include "dojo.h"

Dojo::Dojo(GridPosition pos, int maxHealth) 
    : position(pos), maxHealth(maxHealth), health(maxHealth), isDestroyed(false) {}

Dojo::~Dojo() {}

void Dojo::takeDamage(int amount) {
    health -= amount;
    if (health <= 0) {
        health = 0;
        isDestroyed = true;
    }
}

void Dojo::repair(int amount) {
    health += amount;
    if (health > maxHealth) {
        health = maxHealth;
    }
}

bool Dojo::isDojoDestroyed() const { 
    return isDestroyed; 
}

int Dojo::getHealth() const { 
    return health; 
}

int Dojo::getMaxHealth() const { 
    return maxHealth; 
}

GridPosition Dojo::getPosition() const { 
    return position; 
}

float Dojo::getHealthPercentage() const { 
    return static_cast<float>(health) / maxHealth; 
}