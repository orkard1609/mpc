//Visualization for grid map creation and path planning
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <algorithm>
#include "visualization.hpp"

using namespace std;

//Visualizer stuffs handling
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

    // Calculate final window dimensions with padding
    windowWidth_ = gridWidth_ + (2 * windowPadding) + controlPanelWidth_;
    windowHeight_ = gridHeight_ + (2 * windowPadding);

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
    window_.create(sf::VideoMode(sf::Vector2u(windowWidth_, windowHeight_)), "Motion Planning Visualizer");
    window_.setFramerateLimit(60);
    // Calculate control panel starting position (to the right of the grid)
    boxAlignmentX_ = gridOffsetX_ + gridWidth_ + gridOffsetX_ + 20; // Position after grid + padding
    boxAlignmentY_ = 20; // Starting Y position

    // Grid resize group
    buttons_["labelX"] = {boxAlignmentX_, boxAlignmentY_, 20, 30, "X:", "textOnly", "1"};
    buttons_["inputX"] = {boxAlignmentX_ + 22, boxAlignmentY_, 57, 30, to_string(grid_.getWidth()), "inputBox", "2"};
    buttons_["labelY"] = {boxAlignmentX_ + 81, boxAlignmentY_, 20, 30, "Y:", "textOnly", "3"};
    buttons_["inputY"] = {boxAlignmentX_ + 103, boxAlignmentY_, 57, 30, to_string(grid_.getHeight()), "inputBox", "4"};
    buttons_["resizeConfirm"] = {boxAlignmentX_, boxAlignmentY_ + 40, 160, 30, "Grid size confirmation", "interactiveBox", "5"};
    // Obstacle handling group
    buttons_["setObstacle"] = {boxAlignmentX_, boxAlignmentY_ + 90, 100, 30, "Set obstacle", "interactiveBox", "6"};
    buttons_["undoObstacle"] = {boxAlignmentX_ + 110, boxAlignmentY_ + 90, 50, 30, "Undo", "interactiveBox", "7"};
    buttons_["confirmObstacle"] = {boxAlignmentX_, boxAlignmentY_ + 130, 160, 30, "Confirm obstacle", "interactiveBox", "8"};
    // Start/End points handling
    buttons_["setSE"] = {boxAlignmentX_, boxAlignmentY_ + 180, 160, 30, "Set Start/End points", "interactiveBox", "9"};
    buttons_["confirmSE"] = {boxAlignmentX_, boxAlignmentY_ + 220, 160, 30, "Confirm Start/End points", "interactiveBox", "10"};
    // Path finding algorithm selection
    buttons_["algoLabel"] = {boxAlignmentX_, boxAlignmentY_ + 270, 160, 30, "Path planning algorithm", "textOnly", "11"};
    buttons_["algoInput"] = {boxAlignmentX_, boxAlignmentY_ + 300, 160, 30, "Please select an algo...", "inputBox", "12"};
    // Displaying START/RESET button
    buttons_["startButton"] = {boxAlignmentX_, boxAlignmentY_ + 360, 75, 30, "START", "interactiveBox", "14"};
    buttons_["resetButton"] = {boxAlignmentX_ + 85, boxAlignmentY_ + 360, 75, 30, "RESET", "interactiveBox", "15"};
}

/*
    - Display  the grid
    - Display interactive control buttons and input boxes
*/
void Visualizer::displayWindows() {
    // Display the grid, clear the window with white color
    window_.clear(sf::Color::White);
    // Draw a border around the grid area => Create new method to handle grid rendering
    sf::RectangleShape gridBorder(sf::Vector2f(gridWidth_, gridHeight_));
    gridBorder.setPosition(sf::Vector2f(gridOffsetX_, gridOffsetY_));
    gridBorder.setFillColor(sf::Color(240, 240, 240)); // Light gray background
    gridBorder.setOutlineColor(sf::Color::Black);
    gridBorder.setOutlineThickness(3);
    window_.draw(gridBorder);
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
    // Draw background for control panel on the right => Should be called in drawControlButton()
    sf::RectangleShape controlPanel(sf::Vector2f(200, window_.getSize().y));
    controlPanel.setPosition(sf::Vector2f(gridOffsetX_ + gridWidth_ + gridOffsetX_, 0));
    controlPanel.setFillColor(sf::Color(240, 240, 240)); // Light gray background
    controlPanel.setOutlineColor(sf::Color::Black);
    controlPanel.setOutlineThickness(2);
    window_.draw(controlPanel);
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
vector<pair<int, int>> Visualizer::getCellClick() {
    vector<pair<int, int>> clickedCells;
    // Adjust for grid offset
    int adjustedX = mousePos_x - gridOffsetX_;
    int adjustedY = mousePos_y - gridOffsetY_;
    // Check if mouse position is in grid area or not
    if ((mousePos_x >= gridOffsetX_ && mousePos_x <= gridOffsetX_ + gridWidth_) &&
        (mousePos_y >= gridOffsetY_ && mousePos_y <= gridOffsetY_ + gridHeight_)) {
        // Convert pixel coordinates to grid cell coordinates and ensure coordinates are within grid bounds
        int x = adjustedX / cellSize_;
        x = max(0, min(x, grid_.getWidth() - 1));
        int y = adjustedY / cellSize_;
        y = max(0, min(y, grid_.getHeight() - 1));
        // Add the clicked cell to return vector
        clickedCells.push_back(make_pair(x, y));
        return clickedCells;
        }
    return clickedCells;
}

string Visualizer::getButtonClick() {
    // Check if mouse position is within control buttons area
    if ((mousePos_x >= boxAlignmentX_ && mousePos_x <= boxAlignmentX_ + controlPanelWidth_) &&
        (mousePos_y >= boxAlignmentY_ && mousePos_y <= boxAlignmentY_ + windowHeight_)) {
        for (const auto& [id, button] : buttons_) {
            if (button.isBoxClicked(mousePos_x, mousePos_y)) {
                return id; // Return the ID of the clicked button
            }
        }
    }
    return ""; // Return empty string if no button is clicked
}

/*
This method handles:
    - Get new grid size from the box when "Grid resize confirmation" is clicked
    - Take init grid size and display in the input boxes
    - Remove init grid size when user clicking on the boxes
    - Allow user to remove init value and input new one
    - Pass these value as new grid size and update the grid - improvement for the next version!
*/
void Visualizer::handleGridResize(char inputChar) {
    string clickedButton = getButtonClick();
    static bool f_isActiveXBox = false;
    static bool f_isActiveYBox = false;
    // Handle box click events (activation/deactivation)
    if (clickedButton == "inputX") {
        // If this is the first activation, save current value
        if (inputBoxTextX_.empty()) {
            inputBoxTextX_ = buttons_["inputX"].boxLabel;
            cout << "Saved X value: " << inputBoxTextX_ << endl;
        }
        // Clear the display text to show we're ready for input
        buttons_["inputX"].boxLabel = "";
        f_isActiveXBox = true;
        // Deactivate Y box if it was active, set Y box to previous value
        if (buttons_["inputY"].boxLabel.empty()) {
            buttons_["inputY"].boxLabel = inputBoxTextY_;
            inputBoxTextY_ = "";
            f_isActiveYBox = false;
        }
    }
    else if (clickedButton == "inputY") {
        // If this is the first activation, save current value
        if (inputBoxTextY_.empty()) {
            inputBoxTextY_ = buttons_["inputY"].boxLabel;
            cout << "Saved Y value: " << inputBoxTextY_ << endl;
        }
        // Clear the display text to show we're ready for input
        buttons_["inputY"].boxLabel = "";
        f_isActiveYBox = true;
        // Deactivate X box if it was active, set X box to previous value
        if (buttons_["inputX"].boxLabel.empty()) {
            buttons_["inputX"].boxLabel = inputBoxTextX_;
            inputBoxTextX_ = "";
            f_isActiveXBox = false;
        }
    }
    // Handle keyboard input for active boxes
    if (inputChar != 0) {
        cout << "Received character: " << inputChar << " (ASCII: " << (int)inputChar << ")" << endl;
        // If X box is active (its label is empty)
        if (f_isActiveXBox) {
            if (inputChar == '\b') { // Handle backspace, remove top element from the stack
                if (!inputBoxTextX_.empty()) {
                    inputBoxTextX_.pop_back();
                }
            } 
            else if (isdigit(inputChar) && inputBoxTextX_.size() < 3) { // Limit to 3 digits
                //inputBoxTextX_ += inputChar;
                inputBoxTextX_.push_back(inputChar);
            }
            cout << "Updated X input: '" << inputBoxTextX_ << "'" << endl;
        }
        // If Y box is active (its label is empty)
        else if (f_isActiveYBox) {
            if (inputChar == '\b') { // Handle backspace, remove top element from the stack
                if (!inputBoxTextY_.empty()) {
                    inputBoxTextY_.pop_back();
                }
            } 
            else if (isdigit(inputChar) && inputBoxTextY_.size() < 3) { // Limit to 3 digits
                //inputBoxTextY_ += inputChar;
                inputBoxTextY_.push_back(inputChar);
            }
            cout << "Updated Y input: '" << inputBoxTextY_ << "'" << endl;
        }
    }
    // Handle grid resize confirmation button click
    else if (clickedButton == "resizeConfirm") {
        // Disable X, Y input boxes
        f_isActiveXBox = false;
        f_isActiveYBox = false;
        // Convert new sizes from string to int and parse input values
        int newWidth = !inputBoxTextX_.empty() ? stoi(inputBoxTextX_) : grid_.getWidth();
        int newHeight = !inputBoxTextY_.empty() ? stoi(inputBoxTextY_) : grid_.getHeight();
        // New sizes limitations
        newWidth = max(5, min(newWidth, 100));
        newHeight = max(5, min(newHeight, 100));
        // Resize the grid
        grid_.gridResize(newWidth, newHeight);
        // Update the input boxes to show the confirmed sizes
        buttons_["inputX"].boxLabel = to_string(newWidth);
        buttons_["inputY"].boxLabel = to_string(newHeight);
        // Clear internal input strings
        inputBoxTextX_ = "";
        inputBoxTextY_ = "";
    }
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
    // Text definition is used for all control buttons and overlaped with drawInteractiveBox(), will be optimized later
    sf::Text text(font, inputText, 14);
    text.setFillColor(sf::Color::Black);
    //Center the text in the box
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setPosition(sf::Vector2f(
        x + (width - textBounds.size.x)/2.0f,
        y + (height - textBounds.size.y)/2.0f - textBounds.position.y
    ));
    // Style based on box type
    if (boxType == "textOnly") {
            inputBox.setFillColor(sf::Color::Transparent);
            inputBox.setOutlineColor(sf::Color::Transparent);
            inputBox.setOutlineThickness(0);
    }
    else if (boxType == "inputBox") {
        // Check if this box is currently active for input and enable blinky cursor
        bool isActiveXBox = buttons_["inputX"].boxLabel.empty() && 
                          (x == buttons_["inputX"].x) && 
                          (y == buttons_["inputX"].y);
        bool isActiveYBox = buttons_["inputY"].boxLabel.empty() &&
                          (x == buttons_["inputY"].x) && 
                          (y == buttons_["inputY"].y);
        if (isActiveXBox || isActiveYBox) {
            inputBox.setFillColor(sf::Color::Transparent);
            inputBox.setOutlineColor(sf::Color::Black);
            inputBox.setOutlineThickness(2);
            // Draw the input box first
            window_.draw(inputBox);
            // Reset blinky cursor display
            if (cursorBlinkClock_.getElapsedTime().asSeconds() >= 0.5f) {
                cursorVisible_ = !cursorVisible_;
                cursorBlinkClock_.restart();
            }
            // Get active text based on which box is active
            string activeText = isActiveXBox ? inputBoxTextX_ : inputBoxTextY_;
            // Create and position text
            sf::Text activeInputText(font, activeText, 14);
            activeInputText.setFillColor(sf::Color::Black);
            activeInputText.setPosition(sf::Vector2f(
                x + 10, // Left-aligned text
                y + (height - activeInputText.getLocalBounds().size.y)/2.0f - activeInputText.getLocalBounds().position.y
            ));
            // Draw the active text
            window_.draw(activeInputText);
            // Display blinky cursor
            if (cursorVisible_) {
                sf::RectangleShape cursor(sf::Vector2f(2, height - 6));
                cursor.setFillColor(sf::Color::Black);
                // Calculate text width for cursor positioning after that
                float textWidth = activeText.empty() ? 0 : activeInputText.getLocalBounds().size.x;
                cursor.setPosition(sf::Vector2f(x + 10 + textWidth, y + 3));
                // Draw the cursor
                window_.draw(cursor);
            }            
        } else {
            // Normal styling for inactive input boxes
            inputBox.setFillColor(sf::Color::White);
            inputBox.setOutlineColor(sf::Color::Black);
            inputBox.setOutlineThickness(1);
        }
    }
    else if (boxType == "interactiveBox") {
            inputBox.setFillColor(sf::Color(200, 200, 200)); // Grey color
            inputBox.setOutlineColor(sf::Color::Black);
            inputBox.setOutlineThickness(1);
    }
    // Draw the input box
    window_.draw(inputBox);
    // Draw the text (for non-active input boxes)
    window_.draw(text);
}
//Handle all events
void Visualizer::handleEvents() {
    while (auto optionalEvent = window_.pollEvent()) {
        vector<pair<int, int>> clickedCell;
        const sf::Event& event = *optionalEvent;
        if (event.is<sf::Event::Closed>()) {
            window_.close();
        }
        else if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window_);
                setMousePos(mousePos.x, mousePos.y);
                clickedCell = getCellClick();
                if (!clickedCell.empty()) {
                    cout << "Clicked Cell: (" << clickedCell[0].first << ", " << clickedCell[0].second << ")" << endl;
                } 
                // Process button clicks
                string clickedButton = getButtonClick();
                if (!clickedButton.empty()) {
                    cout << "Button clicked: " << clickedButton << endl;
                    handleGridResize();
                }
                /*if (clickedButton == "inputX" || clickedButton == "inputY" || clickedButton == "resizeConfirm") {
                    handleGridResize();
                }*/
                else if (clickedButton == "algoInput") {
                    handlePathPlanningBox();
                }
                else if (clickedButton == "setObstacle" || clickedButton == "undoObstacle" || clickedButton == "confirmObstacle") {
                    handleSetObstacle();
                    //coloringCell();
                }
                else if (clickedButton == "setSE" || clickedButton == "confirmSE") {
                    handleStartGoalSelection();
                    //coloringCell();
                }
            }
        }
        else if (const auto* textEntered = event.getIf<sf::Event::TextEntered>()) {
            if (textEntered->unicode < 128) { // ASCII characters only
                char inputChar = static_cast<char>(textEntered->unicode);
                // Debug output before handling
                std::cout << "Text entered: '" << inputChar << "' (ASCII: " << (int)inputChar << ")" << std::endl;
                // Pass the character to handleGridResize for processing
                handleGridResize(inputChar);
            }
        }
    }
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
