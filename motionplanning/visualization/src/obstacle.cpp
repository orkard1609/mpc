//Visualization for grid map creation and path planning
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <algorithm>
#include "obstacle.hpp"

using namespace std;

//Obstacle stuffs handling
//Get obstacle positions, get triggered when user clicks on "Set Obstacle" button
vector<pair<int, int>> Obstacle::getObstaclePositions() const {
    return obstaclePositions;
}
//Undo obstacle set operation
void Obstacle::undoObstacle(Grid& grid) {}
//Set obstacle at given position, get triggered when user clicks on "Confirm Obstacle" button, coloring the cells in black
void Obstacle::setObstacle(vector<pair<int, int>> position) {}