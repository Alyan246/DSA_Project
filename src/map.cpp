#include "map.h"
#include "game.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
using namespace std;

Map::Map() {
    WIDTH = sf::VideoMode::getDesktopMode().size.x / 50;
    HEIGHT = sf::VideoMode::getDesktopMode().size.y / 50;

    enemyspawnPoint = GridPosition(35, 7);
    dojoPosition = GridPosition(3, 11);

    grid.resize(WIDTH, vector<int>(HEIGHT, 0)); // ✅ FIX: grid[x][y] means [row][column]

    // block edges
    for (int x = 0; x < WIDTH; x++) {
        grid[x][0] = 1;
        grid[x][1] = 1;
        grid[x][HEIGHT - 1] = 1;
        grid[x][HEIGHT - 2] = 1;
    }
    for (int y = 0; y < HEIGHT; y++) {
        grid[0][y] = 1;
        grid[1][y] = 1;
        grid[WIDTH - 1][y] = 1;
        grid[WIDTH - 2][y] = 1;
    }

    // place random obstacles
    srand(time(0));
    for (int i = 0; i < WIDTH*2; i++) {
        int x = rand() % (WIDTH - 2) + 1;
        int y = rand() % (HEIGHT - 2) + 1;
        if (grid[x][y] == 0 && x != 3 && x != 35 && y!= 7 && y != 11)
            grid[x][y] = 2;
        else
            i--;
    }

    cout << "CALLING FIND" << endl;
    vector<vector<int>> temp1 = grid;
    vector<vector<int>> temp2 = grid;
    vector<vector<int>> temp3 = grid;
    
    findPath(enemyspawnPoint, dojoPosition, 2, temp3);
    findPath(enemyspawnPoint, dojoPosition, 0, temp2);
    findPath(enemyspawnPoint, dojoPosition, 1, temp1);

    vector<vector<bool>> visited(WIDTH, vector<bool>(HEIGHT, false));
    grid[35][7] = 8;
    grid[2][11] = 9;

}

GridPosition Map::getenemyspawn() {
    return enemyspawnPoint;
}

GridPosition Map::getRandomSpawnPosition() {
    int gridWidth = getWidth();
    int gridHeight = getHeight();

    cout << "Map dimensions: " << gridWidth << "x" << gridHeight << endl;

    int spawnX = gridWidth - 1;
    int spawnY = gridHeight / 2;

    if (spawnX >= gridWidth) spawnX = gridWidth - 1;
    if (spawnY >= gridHeight) spawnY = gridHeight - 1;
    if (spawnY < 0) spawnY = 0;

    cout << "Spawn position set to: (" << spawnX << ", " << spawnY << ")" << endl;
    return GridPosition(spawnX, spawnY);
}

bool Map::isValidPosition(int x, int y) const {
    return (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT);
}

bool Map::isValidPath(int x, int y, vector<vector<int>> &temp) const {
    if (isValidPosition(x, y) && temp[x][y] == 0 ) { 
        // cout << "TRUE Want to place at " << x << ',' << y << endl;
        return true;
    }
    // cout << "false " << temp[x][y] << endl;
    return false;
}

bool Map::findPath(const GridPosition &start, const GridPosition &end, int pathfound, vector<vector<int>> &temp) {
    // cout << "Traversing" << endl;
    // cout << "START: " << start.x << " " << start.y << endl;
    // cout << "END  : " << end.x << " " << end.y << endl;

    if (temp[start.x][start.y] == 3) return false;

    // print temp at each traversal
    // for (int x = 0; x < WIDTH; x++) {
    //     for (int y = 0; y < HEIGHT; y++) {
    //         cout << temp[x][y] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;

    if (start.x == end.x && start.y == end.y) {
        temp[start.x][start.y] = 3;

        // cout << "PATH NO. " << pathfound << endl;
        // for (int x = 0; x < WIDTH; x++) {
        //     for (int y = 0; y < HEIGHT; y++) {
        //         cout << temp[x][y] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;

        for (int x = 0; x < WIDTH; x++) {
            for (int y = 0; y < HEIGHT; y++) {
                if (temp[x][y] == 3) grid[x][y] = 3;
            }
        }
        return true;
    }

    if (isValidPath(start.x, start.y, temp) ) {
        temp[start.x][start.y] = 3;
        // cout << "PLACED 3 Successfully at " << start.x << ',' << start.y << endl;

        if(pathfound == 0){
            if (findPath(GridPosition(start.x -1 , start.y), end, pathfound, temp)) return true;
            if (findPath(GridPosition(start.x-1, start.y -1 ), end, pathfound, temp)) return true;
            if (findPath(GridPosition(start.x, start.y+1), end, pathfound, temp)) return true;
            if (findPath(GridPosition(start.x+1, start.y ), end, pathfound, temp)) return true;
        }
        if(pathfound == 1){
            if (findPath(GridPosition(start.x , start.y +1 ), end, pathfound, temp)) return true;
            if (findPath(GridPosition(start.x - 1, start.y ),  end, pathfound, temp)) return true;
            if (findPath(GridPosition(start.x, start.y  -1 ), end, pathfound, temp)) return true;
            if (findPath(GridPosition(start.x + 1 , start.y), end, pathfound, temp)) return true;
        }
        if(pathfound == 2){
            if (findPath(GridPosition(start.x , start.y - 1), end, pathfound, temp)) return true;
            if (findPath(GridPosition(start.x-1, start.y  ), end, pathfound, temp)) return true;
            if (findPath(GridPosition(start.x, start.y+1), end, pathfound, temp)) return true;
            if (findPath(GridPosition(start.x+1, start.y ), end, pathfound, temp)) return true;
        }

        temp[start.x][start.y] = 0;
    }

    return false;
}

Map::~Map() {}

void Map::loadMap(int mapNumber) {
    // For different map designs
}

int Map::getCellType(int x, int y) const {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
        return grid[x][y]; // ✅ consistent with grid[x][y]
    return 1;
}

GridPosition Map::getDojoPosition() const {
    return dojoPosition;
}

int Map::getWidth() const { return WIDTH; }
int Map::getHeight() const { return HEIGHT; }

void Map::calculatePathWeights() {
    // pathfinding helper
}
