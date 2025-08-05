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
vector<pair<int, int>> Obstacle::addObstacle(vector<pair<int, int>>& obstaclePositions, int x, int y) {
    //Create a LIFO stack to store the positions of obstacles
    //Each element is a pair of integers representing the x and y coordinates
    //Whenever the user clicks on a cell to set an obstacle, the position is added to this stack
    //Pseudo code:
    obstaclePositions.push_back({x, y});
    return obstaclePositions; // Return the updated stack of obstacle positions
}

//Undo obstacle set operation
vector<pair<int, int>> Obstacle::undoObstacle(vector<pair<int, int>>& obstaclePositions) {
    //Remove item from the top of the stack
    if (!obstaclePositions.empty()) {
        obstaclePositions.pop_back();
    }  
    return obstaclePositions; // Return the updated stack of obstacle positions
}
