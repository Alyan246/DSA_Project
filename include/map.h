#pragma once
#include <random>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
using namespace std;

class GridPosition{
public:
    int x;
    int y;
    GridPosition(int x = 0, int y = 0) : x(x), y(y) {}
    
};

class Map{
private:
    int WIDTH ;
    int HEIGHT ;
    vector<vector<int>> grid;
    GridPosition dojoPosition;
    GridPosition spawnPoint;
public:
    Map();
    virtual ~Map();
    
    void loadMap(int mapNumber);
    int getCellType(int x, int y) const;
  
    bool isValidPosition(int x, int y) const;
    GridPosition getDojoPosition() const;
    GridPosition getSpawnPoint() const;
    int getWidth() const;
    int getHeight() const;
    GridPosition* findPath(const GridPosition& start, const GridPosition& end, int& pathLength);
    void calculatePathWeights();
};