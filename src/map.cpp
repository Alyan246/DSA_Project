#include "map.h"


Map::Map() :  spawnPoint(1, 7) {
    // Initialize grid with basic path
    WIDTH = sf::VideoMode::getDesktopMode().size.x/50 ;
    HEIGHT = sf::VideoMode::getDesktopMode().size.y/50 ;
    dojoPosition.x = 0;
    dojoPosition.y = HEIGHT/4;
    
    grid.resize(HEIGHT, vector<int>(WIDTH));

    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++) {
            grid[i][j] = 0; // Default to open
        }
    }

    //block edges
    for(int i = 0 ; i < HEIGHT ; i++ ){
        grid[i][0] = 1;
        grid[i][1] = 1;
        grid[i][WIDTH-1] = 1;
        grid[i][WIDTH-2] = 1;
    }

    for(int i = 0 ; i < WIDTH ; i++ ){
        grid[0][i] = 1;
        grid[1][i] = 1;
        grid[HEIGHT-1][i] = 1;
        grid[HEIGHT-2][i] = 1;
    }

    //place trees or random objects
    srand(time(0));
    for(int i = 0 ; i < WIDTH ; i++){
        
        int y = rand() % (HEIGHT - 2) + 1; // row
        int x = rand() % (WIDTH - 2) + 1;  // column
        if (grid[y][x] == 0) {
            grid[y][x] = 2; // obstacle
        } else {
            i--; // retry
        }

        
    }

}

Map::~Map() {}

void Map::loadMap(int mapNumber) {
    //for different map designs
}

int Map::getCellType(int x, int y) const { 
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        return grid[y][x]; 
    }
    return 1; // Return blocked for invalid positions
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
    //path finding Algorithm goes here

    //placeholder path for now:
    pathLength = 2;
    GridPosition* path = new GridPosition[pathLength];
    path[0] = GridPosition(end.x, start.y);
    path[1] = end;
    return path;
}

void Map::calculatePathWeights() { //pathfinding helper function
    // Calculate path weights for enemy decision making
    // This will be used in the pathfinding algorithm
}