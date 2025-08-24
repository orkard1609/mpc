//Visualization for grid map creation and path planning
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <algorithm>
#include "visualization.hpp"

using namespace std;

//Visualizer stuffs handling
//Initializer
Visualizer::Visualizer(Grid& grid, Obstacle& obstacle) 
    : grid_(grid), obstacle_(obstacle) {
    //Set the SFML window with the size based on the grid dimensions
    unsigned int windowWidth = static_cast<unsigned int>(grid_.getWidth()) * static_cast<unsigned int>(cellSize_);
    unsigned int windowHeight = static_cast<unsigned int>(grid_.getHeight()) * static_cast<unsigned int>(cellSize_);
    window_.create(sf::VideoMode(sf::Vector2u(windowWidth, windowHeight)), "Grid map");
    //Set the frame rate limit for the window
    window_.setFramerateLimit(60);
}

//Display empty cells in white
void Visualizer::displayGrid() {
    //Clear the window with white color
    window_.clear(sf::Color::White);
    for (int y = 0; y < grid_.getHeight(); ++y) {
        for (int x = 0; x < grid_.getWidth(); ++x) {
            //Vector2f: a class representing a 2D vector with floating-point coordinates.
            sf::RectangleShape gridCell(sf::Vector2f(cellSize_, cellSize_));
            gridCell.setPosition(sf::Vector2f(x * cellSize_, y * cellSize_));
            gridCell.setOutlineColor(sf::Color::Black);
            gridCell.setOutlineThickness(1);
            gridCell.setFillColor(grid_.isObstacle(x, y) ? sf::Color::Black : sf::Color::White);
            window_.draw(gridCell);
        }
    }
    //Display the drawn cells
    window_.display();
}

// Get the cell click position from the user
void Visualizer::getCellClick(int& x, int& y) {}
//Coloring the cell at (x, y) with the specified color

void Visualizer::undoClick() {
    // Handle undo click event
    // This would typically involve calling the undoObstacle method from the Obstacle class
} 
void Visualizer::resizeClick(int newWidth, int newHeight) {
    // Handle resize click event
    // This would typically involve calling the gridResize method from the Grid class
    if (grid_.isValid(newWidth, newHeight)) {
        grid_.gridResize(newWidth, newHeight);
    } else {
        cout << "Invalid grid size!" << endl;
    }
}

void Visualizer::coloringCell(int x, int y, const string& color) const {
    if (/*getCellClick ==*/ 1) {
        if (color == "white") {
            //Logic to color the cell white
            //Update to black
        } else if (color == "black") {
            // Logic to color the cell black
        }
    }
}

sf::RenderWindow& Visualizer::getWindow() { 
    return window_;
}
