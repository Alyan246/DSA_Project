#pragma once

#include <SFML/Graphics.hpp>

class GridPosition{
public:
    int x;
    int y;
    GridPosition(int x = 0, int y = 0) : x(x), y(y) {}
};

class Map{
private:
    static const int WIDTH = 20;
    static const int HEIGHT = 15;
    int grid[HEIGHT][WIDTH];
    GridPosition dojoPosition;
    GridPosition spawnPoint;

public:
    Map();
    virtual ~Map();
    
    void loadMap(int mapNumber);
    int getCellType(int x, int y) const;
    bool isCellFree(int x, int y) const;
    bool isValidPosition(int x, int y) const;
    GridPosition getDojoPosition() const;
    GridPosition getSpawnPoint() const;
    int getWidth() const;
    int getHeight() const;
    GridPosition* findPath(const GridPosition& start, const GridPosition& end, int& pathLength);
    void calculatePathWeights();
};