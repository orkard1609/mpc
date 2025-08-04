//Visualization for grid map creation and path planning
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <algorithm>
#include "grid.hpp"

using namespace std;

//Grid stuffs handling
//Initializer
Grid::Grid(int width, int height) : width(width), height(height) {
    //Create 2D vector
    gridStatus.resize(height, vector<int>(width, 0));
}
//Resize of the grid
void Grid::gridResize(int newWidth, int newHeight) { 
    //New size validation
    width = newWidth > 0 ? newWidth : 256;
    height = newHeight > 0 ? newHeight : 256;
    //Resize gridStatus to new dimensions
    gridStatus.resize(height, vector<int>(width, 0));
}

bool Grid::isObstacle(int x, int y) const {
    return gridStatus[y][x] == 1; // Assuming 1 indicates an obstacle
} 

void Grid::setObstacle(int x, int y) {
    gridStatus[y][x] = 1; // Set the cell at (x, y) as an obstacle
}

void Grid::clearObstacle(int x, int y) {
    gridStatus[y][x] = 0; // Clear the obstacle at (x, y)
}