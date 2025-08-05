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

//Get input from Obstacle class
void Grid::setObstacle(int x, int y, Obstacle& obstacleStack) {
    for (const auto& pos : obstacleStack.getObstacle()) {
        gridStatus[pos.second][pos.first] = 1; // Set the cell at (x, y) as an obstacle
    }
}

void Grid::clearObstacle(int height, int width) {
    //Clear all obstacles in the grid by setting all cells to 0
    gridStatus.resize(height, vector<int>(width, 0));
}