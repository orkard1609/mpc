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
// Get the cell click position from the user
void Visualizer::getCellClick(int& x, int& y) const {}
//Coloring the cell at (x, y) with the specified color
void Visualizer::coloringCell(int x, int y, const string& color) const {
    if (/*getCellClick ==*/ 1) {
        if (color == "white") {
            // Logic to color the cell white
            //Update to black
        } else if (color == "black") {
            // Logic to color the cell black
        }
    }
}
void Visualizer::undoClick() const {
    // Handle undo click event
    // This would typically involve calling the undoObstacle method from the Obstacle class
} 
void Visualizer::resizeClick(int newWidth, int newHeight) const {
    // Handle resize click event
    // This would typically involve calling the gridResize method from the Grid class
    if (grid.isValid(newWidth, newHeight)) {
        grid.gridResize(newWidth, newHeight);
    } else {
        cout << "Invalid grid size!" << endl;
    }
}

