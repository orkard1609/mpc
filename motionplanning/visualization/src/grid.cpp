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
void Grid::gridResize(int width, int height) { 
    //placeholder for resizing logic
}

bool Grid::isObstacle(int x, int y) const {
    // Placeholder logic for checking if a cell is an obstacle
} 

void Grid::setObstacle(int x, int y) {
    // Placeholder logic for setting an obstacle at (x, y)
}

void Grid::clearObstacle(int x, int y) {
    // Placeholder logic for clearing an obstacle at (x, y)
}