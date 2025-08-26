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
void Visualizer::getCellClick(int& x, int& y) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window_);
    // Convert pixel coordinates to grid cell coordinates and ensure coordinates are within grid bounds
    x = mousePos.x / cellSize_;
    x = std::max(0, std::min(x, grid_.getWidth() - 1));
    y = mousePos.y / cellSize_;
    y = std::max(0, std::min(y, grid_.getHeight() - 1));
}
//Coloring the cell at (x, y) with the specified color

void Visualizer::undoClick() {
    // Handle undo click event
    // Call the undoObstacle method from the Obstacle class
    vector<pair<int, int>> obstaclePositions = obstacle_.getObstacle();
    if (!obstaclePositions.empty()) {
        obstaclePositions = obstacle_.undoObstacle(obstaclePositions);
        // Refresh the grid display after undoing
        displayGrid();
    } else {
        cout << "No obstacles to undo!" << endl;
    }
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
    // Check if the coordinates are valid
    if (x >= 0 && x < grid_.getWidth() && y >= 0 && y < grid_.getHeight()) {
        sf::RectangleShape cell(sf::Vector2f(cellSize_, cellSize_));
        cell.setPosition(sf::Vector2f(x * cellSize_, y * cellSize_));
        cell.setOutlineColor(sf::Color::Black);
        cell.setOutlineThickness(1);
        
        if (color == "white") {
            // Logic to color the cell white (representing empty cell)
            cell.setFillColor(sf::Color::White);
        } else if (color == "black") {
            // Logic to color the cell black (representing obstacle)
            cell.setFillColor(sf::Color::Black);
            // Update the grid with this obstacle
            const_cast<Grid&>(grid_).setObstacle(x, y, const_cast<Obstacle&>(obstacle_));
        } else if (color == "green") {
            // Optional: Logic for start position
            cell.setFillColor(sf::Color::Green);
        } else if (color == "red") {
            // Optional: Logic for goal position
            cell.setFillColor(sf::Color::Red);
        } else if (color == "blue") {
            // Optional: Logic for path
            cell.setFillColor(sf::Color::Blue);
        }
        
        // Draw the cell on the window
        const_cast<sf::RenderWindow&>(window_).draw(cell);
        const_cast<sf::RenderWindow&>(window_).display();
    }
}

sf::RenderWindow& Visualizer::getWindow() { 
    return window_;
}
