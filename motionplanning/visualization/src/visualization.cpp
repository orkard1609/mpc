//Visualization for grid map creation and path planning
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <algorithm>
#include "grid.hpp"
#include "obstacle.hpp"
#include "visualization.hpp"

using namespace std;

//Visualizer stuffs handling
//Display empty cells in white
void Visualizer::displayGrid() const {}
//Display obstacles in black
void Visualizer::displayObstacles() const {}
//Display path in green
void Visualizer::displayPath() const {}
//Handle user input
void Visualizer::handleUserInput() const {}
//Display status, check if the grid is open for interaction
bool Visualizer::gridisOpen() const {}

