#include "map.h"

Map::Map() : dojoPosition(18, 7), spawnPoint(1, 7) {
    // Initialize grid with basic path
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++) {
            grid[i][j] = 1; // Default to blocked
        }
    }
    
    // Create a simple horizontal path
    for(int j = 1; j < WIDTH - 1; j++) {
        grid[7][j] = 0; // Free path
    }
}

Map::~Map() {}

void Map::loadMap(int mapNumber) {
    // Load different map layouts based on mapNumber
    // This will be implemented with the actual map designs
}

int Map::getCellType(int x, int y) const { 
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        return grid[y][x]; 
    }
    return 1; // Return blocked for invalid positions
}

bool Map::isCellFree(int x, int y) const { 
    return getCellType(x, y) == 0; 
}

bool Map::isValidPosition(int x, int y) const { 
    return (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT); 
}

GridPosition Map::getDojoPosition() const { 
    return dojoPosition; 
}

GridPosition Map::getSpawnPoint() const { 
    return spawnPoint; 
}

int Map::getWidth() const { 
    return WIDTH; 
}

int Map::getHeight() const { 
    return HEIGHT; 
}

GridPosition* Map::findPath(const GridPosition& start, const GridPosition& end, int& pathLength) {
    // Placeholder for A* pathfinding algorithm
    // For now, return a simple direct path
    pathLength = 2;
    GridPosition* path = new GridPosition[pathLength];
    path[0] = GridPosition(end.x, start.y);
    path[1] = end;
    return path;
}

void Map::calculatePathWeights() {
    // Calculate path weights for enemy decision making
    // This will be used in the pathfinding algorithm
}