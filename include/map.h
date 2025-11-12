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
    GridPosition enemyspawnPoint;
public:
    Map();
    virtual ~Map();
    
    void loadMap(int mapNumber);
    int getCellType(int x, int y) const;
    bool isValidPath(int x, int y, std::vector<std::vector<int>>& tgrid) const ; // looks at the path being genrated to see if block can be part of a possible path
    bool isValidPosition(int x, int y) const; // checks if in map 
    GridPosition getDojoPosition() const;
    
    
    int getWidth() const;
    int getHeight() const;
    bool findPath(const GridPosition& start, const GridPosition& end, int pathfound, std::vector<std::vector<int>>& tempgrid);
    void calculatePathWeights();
    GridPosition getRandomSpawnPosition();
    GridPosition getenemyspawn();
};