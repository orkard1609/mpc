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

    // Next step: make all these properties adjustable based on grid size input only
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

    // Calculate control panel starting position (to the right of the grid)
    int boxAlignmentX = gridOffsetX_ + gridWidth_ + gridOffsetX_ + 20; // Position after grid + padding
    int boxAlignmentY = 20; // Starting Y position

    // Init box/button map
    // Grid resize group
    buttons_["labelX"] = {boxAlignmentX, boxAlignmentY, 20, 30, "X:", "textOnly", "1"};
    buttons_["inputX"] = {boxAlignmentX + 22, boxAlignmentY, 57, 30, to_string(grid_.getWidth()), "inputBox", "2"};
    buttons_["labelY"] = {boxAlignmentX + 81, boxAlignmentY, 20, 30, "Y:", "textOnly", "3"};
    buttons_["inputY"] = {boxAlignmentX + 103, boxAlignmentY, 57, 30, to_string(grid_.getHeight()), "inputBox", "4"};
    buttons_["resizeConfirm"] = {boxAlignmentX, boxAlignmentY + 40, 160, 30, "Grid size confirmation", "interactiveBox", "5"};
    // Obstacle handling group

    buttons_["setObstacle"] = {boxAlignmentX, boxAlignmentY + 90, 100, 30, "Set obstacle", "interactiveBox", "6"};
    buttons_["undoObstacle"] = {boxAlignmentX + 110, boxAlignmentY + 90, 50, 30, "Undo", "interactiveBox", "7"};
    buttons_["confirmObstacle"] = {boxAlignmentX, boxAlignmentY + 130, 160, 30, "Confirm obstacle", "interactiveBox", "8"};
    // Start/End points handling
    buttons_["setSE"] = {boxAlignmentX, boxAlignmentY + 180, 160, 30, "Set Start/End points", "interactiveBox", "9"};
    buttons_["confirmSE"] = {boxAlignmentX, boxAlignmentY + 220, 160, 30, "Confirm Start/End points", "interactiveBox", "10"};
    // Path finding algorithm selection
    buttons_["algoLabel"] = {boxAlignmentX, boxAlignmentY + 270, 160, 30, "Path planning algorithm", "textOnly", "11"};
    buttons_["algoInput"] = {boxAlignmentX, boxAlignmentY + 300, 160, 30, "Please select an algo...", "inputBox", "12"};
    
    
    // Displaying START/RESET button
    buttons_["startButton"] = {boxAlignmentX, boxAlignmentY + 360, 75, 30, "START", "interactiveBox", "14"};
    buttons_["resetButton"] = {boxAlignmentX + 85, boxAlignmentY + 360, 75, 30, "RESET", "interactiveBox", "15"};

    // Get mouse position on the displaying window
    mousePos_ = sf::Mouse::getPosition(window_);
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

    // Dislaying input box and text for grid resize
    for (const auto& [id, button] : buttons_) {
        drawControlButton(button.x, button.y, button.width, button.height, button.boxLabel, button.boxType);
    }
    
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
    - Take init grid size and display in the input boxes
    - Display the cursor when user clicking on the boxes
    - Allow user to remove init value and input new one
    - Pass these value as new grid size and update the grid - improvement for the next version!
*/
void Visualizer::handleGridResize(int newWidth, int newHeight) {

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
    - Displaying "X, Y" text and input box for new grid size
    - Displaying "Path finding algorithm" text
    - Getting list of supported path finding algorithms and displayed as drop out list
    - Getting user selected path finding algorithm and send it to MPAlgo object
*/
void Visualizer::drawControlButton(int x, int y, int width, int height, const std::string& inputText, const std::string& boxType) {
    // Create the input box shape
    sf::RectangleShape inputBox(sf::Vector2f(width, height));
    inputBox.setPosition(sf::Vector2f(x, y));

    // Text definition is overlaped with drawInteractiveBox(), will be optimized later
    sf::Text text(font, inputText, 14);
    text.setFillColor(sf::Color::Black);
    //Center the text in the box
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setPosition(sf::Vector2f(
        x + (width - textBounds.size.x)/2.0f,
        y + (height - textBounds.size.y)/2.0f - textBounds.position.y
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
    else if (boxType == "interactiveBox") {
            inputBox.setFillColor(sf::Color(200, 200, 200)); // Grey color
            inputBox.setOutlineColor(sf::Color::Black);
            inputBox.setOutlineThickness(1);
    }

    // Draw the input box
    window_.draw(inputBox);
    // Draw the text
    window_.draw(text);
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
