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
    // Calculate the grid dimensions in pixels
    gridWidth_ = static_cast<unsigned int>(grid_.getWidth()) * static_cast<unsigned int>(cellSize_);
    gridHeight_ = static_cast<unsigned int>(grid_.getHeight()) * static_cast<unsigned int>(cellSize_);

    //Next step: make all these properties adjustable based on grid size input only
    // Add padding around the grid area
    unsigned int windowPadding = 10; 
    // Store the padding value for use in drawing the grid
    gridOffsetX_ = windowPadding;
    gridOffsetY_ = windowPadding;
    // Add control panel to the right
    unsigned int controlPanelWidth = 200;
    // Calculate final window dimensions with padding
    unsigned int windowWidth = gridWidth_ + (2 * windowPadding) + controlPanelWidth;
    unsigned int windowHeight = gridHeight_ + (2 * windowPadding);

    //Font loaded as global property
    static bool isFontLoaded = false;
    if (!isFontLoaded) {
        if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
            // Handle font loading error
            cerr << "Could not load font!" << endl;
            return;
        }
        isFontLoaded = true;
    }

    // Create the window
    window_.create(sf::VideoMode(sf::Vector2u(windowWidth, windowHeight)), "Motion Planning Visualizer");
    window_.setFramerateLimit(60);
}

/*
    - Display  the grid
    - Display interactive control buttons and input boxes
*/
void Visualizer::displayWindows() {
    // Display the grid, clear the window with white color
    window_.clear(sf::Color::White);
    
    // Draw a border around the grid area
    sf::RectangleShape gridBorder(sf::Vector2f(gridWidth_, gridHeight_));
    gridBorder.setPosition(sf::Vector2f(gridOffsetX_, gridOffsetY_));
    gridBorder.setFillColor(sf::Color(240, 240, 240)); // Light gray background
    gridBorder.setOutlineColor(sf::Color::Black);
    gridBorder.setOutlineThickness(3);
    window_.draw(gridBorder);
    
    // Draw background for control panel on the right
    sf::RectangleShape controlPanel(sf::Vector2f(200, window_.getSize().y));
    controlPanel.setPosition(sf::Vector2f(gridOffsetX_ + gridWidth_ + gridOffsetX_, 0));
    controlPanel.setFillColor(sf::Color(240, 240, 240)); // Light gray background
    controlPanel.setOutlineColor(sf::Color::Black);
    controlPanel.setOutlineThickness(2);
    window_.draw(controlPanel);
    
    // Draw grid cells within the grid area (with offset from window edges)
    for (int y = 0; y < grid_.getHeight(); ++y) {
        for (int x = 0; x < grid_.getWidth(); ++x) {
            //Vector2f: a class representing a 2D vector with floating-point coordinates.
            sf::RectangleShape gridCell(sf::Vector2f(cellSize_, cellSize_));
            gridCell.setPosition(sf::Vector2f(
                gridOffsetX_ + (x * cellSize_),
                gridOffsetY_ + (y * cellSize_)
            ));
            gridCell.setOutlineColor(sf::Color::Black);
            gridCell.setOutlineThickness(1);
            gridCell.setFillColor(grid_.isObstacle(x, y) ? sf::Color::Black : sf::Color::White);
            window_.draw(gridCell);
        }
    }
    
    // Calculate control panel starting position (to the right of the grid)
    int boxAlignmentX = gridOffsetX_ + gridWidth_ + gridOffsetX_ + 20; // Position after grid + padding
    int boxAlignmentY = 20; // Starting Y position

    // Dislaying input box and text for grid resize
    drawInputBox(boxAlignmentX, boxAlignmentY, 20, 30, "X:", "textOnly"); // X input box
    drawInputBox(boxAlignmentX + 22, boxAlignmentY, 57, 30, "", "inputBox"); // Get new grid width
    drawInputBox(boxAlignmentX + 81, boxAlignmentY, 20, 30, "Y:", "textOnly"); // Y input box
    drawInputBox(boxAlignmentX + 103, boxAlignmentY, 57, 30, "", "inputBox"); // Get new grid height
    // Displaying and alignment control boxes on the right panel
    drawInteractiveBox(boxAlignmentX, boxAlignmentY + 40, 160, 30, "Grid size confirmation");
    drawInteractiveBox(boxAlignmentX, boxAlignmentY + 90, 100, 30, "Set obstacle");
    drawInteractiveBox(boxAlignmentX + 110, boxAlignmentY + 90, 50, 30, "Undo");
    drawInteractiveBox(boxAlignmentX, boxAlignmentY + 130, 160, 30, "Confirm obstacle");
    drawInteractiveBox(boxAlignmentX, boxAlignmentY + 180, 160, 30, "Set start/end points");
    drawInteractiveBox(boxAlignmentX, boxAlignmentY + 220, 160, 30, "Confirm start/end points");
    // Displaying input boxes
    drawInputBox(boxAlignmentX, boxAlignmentY + 270, 160, 30, "Path planning algorithm", "textOnly"); // Path planning algorithm input box
    drawInputBox(boxAlignmentX, boxAlignmentY + 300, 160, 30, "", "inputBox"); // This box will get user selected algo
    drawInputBox(boxAlignmentX, boxAlignmentY + 300, 160, 30, "Please select an algo...", "textOnly"); // Display text only

    // Displaying START/RESET button
    drawInteractiveBox(boxAlignmentX, boxAlignmentY + 360, 75, 30, "START");
    drawInteractiveBox(boxAlignmentX + 85, boxAlignmentY + 360, 75, 30, "RESET");

    // Display the windows, including grid, control buttons and input boxes
    window_.display();
}

/*
This method helps:
    - Get the cell click position from the user on the grid
*/
void Visualizer::getCellClick(int& x, int& y) {
    //2D vector with integer coordinates
    sf::Vector2i mousePos = sf::Mouse::getPosition(window_);
    
    // Adjust for grid offset
    int adjustedX = mousePos.x - gridOffsetX_;
    int adjustedY = mousePos.y - gridOffsetY_;
    
    // Convert pixel coordinates to grid cell coordinates and ensure coordinates are within grid bounds
    x = adjustedX / cellSize_;
    x = max(0, min(x, grid_.getWidth() - 1));
    y = adjustedY / cellSize_;
    y = max(0, min(y, grid_.getHeight() - 1));
}

/*
This method handles:
    - Get new grid size from the box when "Grid resize confirmation" is clicked
*/
void Visualizer::handleGridResize(int newWidth, int newHeight) {
    //Place-holder
}

/*
This method handles:
    - Get "Set obstacle set" click event -> Push new obstacle location to the obstacle stack
    - Get "Undo" click event -> Pop last obstacle location from the obstacle stack
    - Get "Confirm obstacle" click event -> Set the obstacles in the grid based on the obstacle stack
*/
void Visualizer::handleSetObstacle() {
    //Place-holder
}

/*
This method handles:
    - Get "Start/End" click event -> Ready to get start/end position
    - Get "Confirm Start/End" click event -> Set the start/end position
*/
void Visualizer::handleStartGoalSelection() {
    //Place-holder
}

/*
This method handles:
    - Display path planning algorithms as drop out list
    - Get selected path planning algorithms
*/
void Visualizer::handlePathPlanningBox() {
    //Place-holder
}

/*
This method helps:
    - Definition all properties of interactive boxes
*/
void Visualizer::drawInteractiveBox(int x, int y, int width, int height, const std::string& label) {
    //Create the box style
    sf::RectangleShape box(sf::Vector2f(width, height));
    box.setPosition(sf::Vector2f(x, y));
    box.setFillColor(sf::Color(200, 200, 200)); // Grey color
    box.setOutlineColor(sf::Color::Black);
    box.setOutlineThickness(1);
    //Text style, construct with font for SFML 3.0.0, text(label, font, size)
    sf::Text text(font, label, 14);
    text.setFillColor(sf::Color::Black);
    //Center the text in the box
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setPosition(sf::Vector2f(
        x + (width - textBounds.size.x)/2.0f,
        y + (height - textBounds.size.y)/2.0f - textBounds.position.y
    )); // Add some padding    
    //Draw the box
    window_.draw(box);
    //Draw the text
    window_.draw(text);
}
/*
This method helps:
    - Displaying "X, Y" text and input box for new grid size
    - Displaying "Path finding algorithm" text
    - Getting list of supported path finding algorithms and displayed as drop out list
    - Getting user selected path finding algorithm and send it to MPAlgo object
*/
void Visualizer::drawInputBox(int x, int y, int width, int height, const std::string& inputText, const std::string& boxType) {
    // Create the input box shape
    sf::RectangleShape inputBox(sf::Vector2f(width, height));
    inputBox.setPosition(sf::Vector2f(x, y));

    // Text definition is overlaped with drawInteractiveBox(), will be optimized later
    sf::Text inputBoxText(font, inputText, 14);
    inputBoxText.setFillColor(sf::Color::Black);
    //Center the text in the box
    sf::FloatRect inputBoxTextBounds = inputBoxText.getLocalBounds();
    inputBoxText.setPosition(sf::Vector2f(
        x + (width - inputBoxTextBounds.size.x)/2.0f,
        y + (height - inputBoxTextBounds.size.y)/2.0f - inputBoxTextBounds.position.y
    )); // Add some padding    

    if (boxType == "textOnly") {
            inputBox.setFillColor(sf::Color::Transparent);
            inputBox.setOutlineColor(sf::Color::Transparent);
            inputBox.setOutlineThickness(0);

    }
    else if (boxType == "inputBox") {
            inputBox.setFillColor(sf::Color::White);
            inputBox.setOutlineColor(sf::Color::Black);
            inputBox.setOutlineThickness(1);
    }

    // Draw the input box
    window_.draw(inputBox);
    // Draw the text
    window_.draw(inputBoxText);
}

/*
This method helps:
    - Checking if a specific box is clicked
*/
bool Visualizer::isBoxClicked(int x, int y, int width, int height) const {
    //Get mouse position on the displaying window
    sf::Vector2i mousePos = sf::Mouse::getPosition(window_);
    //Check if current mouse position is in the specified location
    return (mousePos.x >= x && mousePos.x <= x + width &&
            mousePos.y >= y && mousePos.y <= y + height);
}

/*
This method helps:
    - Coloring empty cell as White
    - Coloring obstacle cell as Black
    - Coloring start cell as Green
    - Coloring goal cell as Red
    - Coloring path cell as Blue
*/
void Visualizer::coloringCell(int x, int y, const string& color) const {
    // Check if the coordinates are valid
    if (x >= 0 && x < grid_.getWidth() && y >= 0 && y < grid_.getHeight()) {
        sf::RectangleShape cell(sf::Vector2f(cellSize_, cellSize_));
        cell.setPosition(sf::Vector2f(gridOffsetX_ + (x * cellSize_), gridOffsetY_ + (y * cellSize_)));
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
