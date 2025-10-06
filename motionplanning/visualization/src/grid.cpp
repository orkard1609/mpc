//Visualization for grid map creation and path planning
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <algorithm>
#include "grid.hpp"
#include "obstacle.hpp"

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
//Set cell state based on obstacle, start and goal positions
void Grid::setCellState(int x, int y, Grid::cellStatus status) {
    gridStatus[y][x] = static_cast<int>(status); // Update the cell status
}
//Get cell state
Grid::cellStatus Grid::getCellState(int x, int y) const {
    Grid::cellStatus status = static_cast<Grid::cellStatus>(gridStatus[y][x]);
    return status;
}