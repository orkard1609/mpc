//Visualization for grid map creation and path planning
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <algorithm>
#include "visualization.hpp"

using namespace std;

// Draw input box with text and blinky cursor for grid resize input boxes
void drawBlinkyCursor(sf::RenderWindow& window_,
                      sf::RectangleShape& inputBox,
                      bool isABoxActive,
                      sf::Clock& cursorBlinkClock_,
                      bool& cursorVisible_,
                      const string& inputBoxTextX_,
                      const string& inputBoxTextY_,
                      int x, int y, int height,
                      sf::Font& font) {
    // Check if this box is currently active for input and enable blinky cursor
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
    string activeText = isABoxActive ? inputBoxTextX_ : inputBoxTextY_;
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
}

// Draw dropdown list for path planning algorithm selection
void drawAlgorithmDropdownList(sf::RenderWindow& window_,
                               map<string, controlButton> buttons_,
                               sf::RectangleShape& inputBox,
                               vector<string> algorithms,
                               int x, int y,
                               int width, int height,
                               sf::Font& font,
                               string& clickedAlgo_,
                               bool& clickedOutside_,
                               int mousePos_x, int mousePos_y) {
    int newHeight = height * (algorithms.size() + 1); // +1 for the main box
    inputBox.setSize(sf::Vector2f(width, newHeight));
    inputBox.setFillColor(sf::Color::Transparent);
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setOutlineThickness(1);
    // Draw the input box first
    window_.draw(inputBox);

    // Clicking outside of the list detection
    clickedOutside_ = (mousePos_x >= x && mousePos_x <= x + width &&
                       mousePos_y >= y && mousePos_y <= y + newHeight) ? false : true;
    
    // Display algorithm name in drop down list
    for (int i = 0; i < algorithms.size(); i++) {  
        // Create a temporary rectangle for this item
        sf::RectangleShape itemBox(sf::Vector2f(width, height));
        itemBox.setPosition(sf::Vector2f(x, y + height * (i + 1)));
        
        // Check if mouse is over this item (for highlighting), using <, > to avoid border issues
        bool isHovered = (mousePos_x >= x && mousePos_x <= x + width &&
                          mousePos_y > y + height * (i + 1) && 
                          mousePos_y < y + height * (i + 2));

        // Set fill color (highlight on hover) and  return its index
        itemBox.setFillColor(isHovered ? sf::Color(200, 240, 200) : sf::Color(240, 240, 240));
        
        // Draw the item background
        window_.draw(itemBox);

        // Draw the item text
        sf::Text itemText(font, algorithms[i], 14);
        itemText.setFillColor(sf::Color::Black);
        itemText.setPosition(sf::Vector2f(
            x + 10,
            y + height * (i + 1) + (height - itemText.getLocalBounds().size.y)/2.0f
        ));
        window_.draw(itemText);  
   
        // If mouse clicked on this item, return selected algorithm
        if (isHovered) {
            clickedAlgo_ = algorithms[i];
        }     
    }
}
// Constructor to initialize visualizer with grid and obstacle
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
    //buttons_["algoInput"] = {boxAlignmentX_, boxAlignmentY_ + 300, 160, 30, "Please select an algo...", "inputBox", "12"};
    buttons_["algoInput"] = {boxAlignmentX_, boxAlignmentY_ + 300, 160, 30, selectedAlgo_, "inputBox", "12"};
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
            gridCell.setOutlineColor(sf::Color(150, 150, 150)); // Light gray outline
            gridCell.setOutlineThickness(1);
            coloringCell(x, y, cellColor);
            gridCell.setFillColor(cellColor);
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
        // Skip START/RESET buttons when dropdown is open
        if (isAlgoDropdownOpen_ && (id == "startButton" || id == "resetButton")) {
            continue;         
        }
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
            // Handle backspace, remove top element from the stack
            if (inputChar == '\b') { 
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
    if (clickedButton == "setObstacle") {
        // Toggle obstacle setting mode only
        isObstacleSet_ = !isObstacleSet_;
    } else if (isObstacleSet_) {
        if (!clickedCell.empty()) {
            // Set clicked cell as obstacle
            grid_.setCellState(clickedCell[0].first, clickedCell[0].second, Grid::OBSTACLE); 
            // Add the clicked cell to obstacle stack
            obstacle_.addObstacle(obstacleStack_, clickedCell[0].first, clickedCell[0].second);
        }
        if (clickedButton == "undoObstacle") {
            // Undo last obstacle setting
            grid_.setCellState(obstacleStack_.back().first, obstacleStack_.back().second, Grid::EMPTY);
            obstacle_.undoObstacle(obstacleStack_);
            // Safe return to avoid popping from empty stack
            if (obstacleStack_.empty()) {
                isObstacleSet_ = false;
                return;
            }
        } else if (clickedButton == "confirmObstacle") {
            isObstacleSet_ = false; // Exit obstacle setting mode
            // Print confirmed obstacle stack
            cout <<  "Obstacle stack:" << endl;
            if (obstacleStack_.empty()) {
                cout << "No obstacles set." << endl;
                return;
            } else {
                for (const auto& cell : obstacleStack_) {
                    cout << "(" << cell.first << ", " << cell.second << ")," << endl;
                }
            }
        }
    }
}
/*
This method handles:
    - Get "Start/End" click event -> Ready to get start/end position
    - Get "Confirm Start/End" click event -> Set the start/end position
*/
void Visualizer::handleStartGoalSelection() {
    if (clickedButton == "setSE") {
        // Toggle start/goal setting mode only
        isStartGoalSet_ = !isStartGoalSet_;
    } else if (isStartGoalSet_) {
        if (!clickedCell.empty()) {
            // Ensure start and goal positions are set only once
            if (selectedStartGoal_.empty()) {
                selectedStartGoal_.push_back({clickedCell[0].first, clickedCell[0].second}); // Set start
                grid_.setCellState(selectedStartGoal_.back().first, selectedStartGoal_.back().second, Grid::START);

            } else if (selectedStartGoal_.size() == 1) {
                // Prevent setting goal on the same cell as start
                if (clickedCell[0] != selectedStartGoal_.back()) {
                    selectedStartGoal_.push_back({clickedCell[0].first, clickedCell[0].second}); // Set goal
                    grid_.setCellState(selectedStartGoal_.back().first, selectedStartGoal_.back().second, Grid::GOAL);
                } else {
                    cout << "Goal cannot be the same as START. Please select a different cell." << endl;
                }
            } else {
                cout << "Both Start and Goal positions are already set. Please confirm or reset." << endl;
            }
        }
        if (clickedButton == "confirmSE") {
            // Ensure both start and goal positions are set before confirming
            if (selectedStartGoal_.size() == 2) {
                isStartGoalSet_ = false; // Exit start/goal setting mode
                cout << "Start position: (" << selectedStartGoal_[0].first << ", " << selectedStartGoal_[0].second << ")" << endl;
                cout << "Goal position: (" << selectedStartGoal_[1].first << ", " << selectedStartGoal_[1].second << ")" << endl;
            } else {
                cout << "Please set both Start and Goal positions before confirming." << endl;
            }
        }
    }
}

/*
This method handles:
    - Display path planning algorithms as drop out list
    - Get selected path planning algorithms
*/
void Visualizer::handlePathPlanningBox() {
    /*
    Sequence:
    1. Init, show as "Dijkstra"
    2. If user click on that, clear label and show drop-down list
    2.1. If user doesn't select anything and click outside the drop-down list => Set back to "Dijkstra"
    2.2. If user select an algorithm in the list => pass that algorithm into selectedAlgo_
    3. Close the list 
    */
    if (clickedButton == "algoInput") {
        // If this is the first activation, save current value, toggle dropdown when clicking the main button
        isAlgoDropdownOpen_ = !isAlgoDropdownOpen_;
        // Clear algo selection box if dropdown is open
        buttons_["algoInput"].boxLabel = isAlgoDropdownOpen_ ? "" : selectedAlgo_;
        return;
    }
    // If an algorithm is selected from the drop-down list, update selectedAlgo_ and close the list
    if (clickedAlgo_ != "") {
        selectedAlgo_ = clickedAlgo_;
        buttons_["algoInput"].boxLabel = selectedAlgo_;
        // Clear clickedAlgo_ to avoid re-setting the same algo
        clickedAlgo_ = "";
        // Close the drop-down list
        isAlgoDropdownOpen_ = false;
    } else if (clickedOutside_) {
        // Set back to init algorithm and close the list if click outside drop-down list area without selecting an algorithm is detected
        buttons_["algoInput"].boxLabel = selectedAlgo_;
        isAlgoDropdownOpen_ = false;
    }
}

/*
This method helps:
    - Get found path from MPAlgo class
*/
void Visualizer::handlePathPlanningAlgo() {
    // Clear any existing path data first to avoid showing stale paths
    path_.clear();
    visitedNodes_.clear();
    
    if (!selectedAlgo_.empty() && selectedStartGoal_.size() == 2) {
        isPathDisplaying_ = true;
        
        // Get the pathfinding algorithm instance
        auto pathFinding = PathFindingAlgorithm::selectAlgorithm(selectedAlgo_, grid_, selectedStartGoal_[0], selectedStartGoal_[1]);
        
        // Only proceed if we have a valid algorithm
        if (pathFinding) {
            // Get the path from the algorithm
            path_ = pathFinding->getFoundPath();
            visitedNodes_ = pathFinding->getVisitedNodes();
            // Display visited nodes first
            if (!visitedNodes_.empty()) {
                for (const auto& node : visitedNodes_) {
                    // Mark cells as VISITED
                    if (node != selectedStartGoal_[0] && node != selectedStartGoal_[1]) {
                        grid_.setCellState(node.first, node.second, Grid::VISITED);
                    }
                }
                cout << endl;
            }
            // Display the path if it's not empty
            if (!path_.empty()) {
                for (const auto& p : path_) {
                    // Mark cells as PATH
                    if (p != selectedStartGoal_[0] && p != selectedStartGoal_[1]) {
                        grid_.setCellState(p.first, p.second, Grid::PATH);
                    }
                }
                cout << endl;
            } else {
                cout << "No path found between start and goal points." << endl;
                isPathDisplaying_ = false;
            }
        } else {
            cout << "Failed to initialize pathfinding algorithm." << endl;
            isPathDisplaying_ = false;
        }
    }
    else {
        cout << "Please select Start and Goal position first!" << endl;
        isPathDisplaying_ = false;
    }
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
        // Need to increase the majurity of box activation check
        bool isActiveXBox = buttons_["inputX"].boxLabel.empty() && 
                            (x == buttons_["inputX"].x) && 
                            (y == buttons_["inputX"].y);
        bool isActiveYBox = buttons_["inputY"].boxLabel.empty() &&
                            (x == buttons_["inputY"].x) && 
                            (y == buttons_["inputY"].y);
        bool isActiveAlgoSelection = buttons_["algoInput"].boxLabel.empty() &&
                                     (x == buttons_["algoInput"].x) && 
                                     (y == buttons_["algoInput"].y);
        if (isActiveXBox || isActiveYBox) {
            drawBlinkyCursor(window_, inputBox, isActiveXBox,
                             cursorBlinkClock_, cursorVisible_, 
                             inputBoxTextX_, inputBoxTextY_, 
                             x, y, height, font);
        } 
        else if (isActiveAlgoSelection && isAlgoDropdownOpen_) {
            // Call drawAlgorithmDropdownList for rendering when "algoInput" label is empty
            drawAlgorithmDropdownList(window_, buttons_, inputBox,
                                      algorithms,
                                      x, y, width, height,
                                      font, clickedAlgo_, clickedOutside_,
                                      mousePos_x, mousePos_y);
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

// Reset windows to init state
void Visualizer::resetWindows() {
    // Reset the grid and related variables to initial state
    //grid_.gridResize(150, 150); // Reset to default size
    grid_.gridResize(20, 20);
    // Set all cells to EMPTY
    for (int y = 0; y < grid_.getHeight(); ++y) {
        for (int x = 0; x < grid_.getWidth(); ++x) {
            grid_.setCellState(x, y, Grid::EMPTY);
        }
    }
    
    // Then randomly setting obstacles for testing - FOR TESTING ONLY
    srand(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < 5; ++i) {
    //for (int i = 0; i < 4000; ++i) {
        int x = rand() % grid_.getWidth();
        int y = rand() % grid_.getHeight();
        grid_.setCellState(x, y, Grid::OBSTACLE);
    }

    // Set all attributes back to init state
    obstacleStack_.clear();
    selectedStartGoal_.clear();
    path_.clear(); // Clear any path data
    visitedNodes_.clear();
    isObstacleSet_ = false;
    isStartGoalSet_ = false;
    isPathDisplaying_ = false; // Reset path display state
    selectedAlgo_ = "Dijkstra";
    buttons_["inputX"].boxLabel = to_string(grid_.getWidth());
    buttons_["inputY"].boxLabel = to_string(grid_.getHeight());
    buttons_["algoInput"].boxLabel = selectedAlgo_;
    inputBoxTextX_ = "";
    inputBoxTextY_ = "";
    clickedAlgo_ = "";
    isAlgoDropdownOpen_ = false;
    cout << "Grid and settings have been reset to initial state." << endl;
}
//Handle all events
void Visualizer::handleEvents() {
    while (auto optionalEvent = window_.pollEvent()) {
        const sf::Event& event = *optionalEvent;
        if (event.is<sf::Event::Closed>()) {
            window_.close();
        }
        else if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window_);
                setMousePos(mousePos.x, mousePos.y);
                clickedCell = getCellClick();
                // Process button clicks
                clickedButton = getButtonClick();
                if (!clickedButton.empty()) {
                    cout << "Button clicked: " << clickedButton << endl;
                }
                if (clickedButton == "inputX" || clickedButton == "inputY" || clickedButton == "resizeConfirm") {
                    handleGridResize();
                }
                // Call handlePathPlanningBox() if algoInput box or drop-down list is open
                else if (clickedButton == "algoInput" || isAlgoDropdownOpen_) {
                    handlePathPlanningBox();
                    // Print selected algorithm for debugging
                    cout << "Selected algorithm: " << selectedAlgo_ << endl;
                }
                else if (clickedButton == "setObstacle" || isObstacleSet_) {
                    handleSetObstacle();
                }
                else if (clickedButton == "setSE" || isStartGoalSet_) {
                    handleStartGoalSelection();
                }
                else if (clickedButton == "startButton") {
                    handlePathPlanningAlgo();
                }
                else if (clickedButton == "resetButton") {
                    resetWindows();
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
    - Coloring empty cell as White, obstacle cell as Black, start cell as Blue, goal cell as Red, path cell as Green, visited node as Magenta
*/
void Visualizer::coloringCell(int x, int y, sf::Color& cellColor) const {
    // Set colors based on cell state
    if (grid_.getCellState(x, y) == Grid::OBSTACLE) {
        // Obstacle cell
        cellColor = sf::Color::Black;
    } else if (grid_.getCellState(x, y) == Grid::START) {
        // Start cell
        cellColor = sf::Color::Blue;
    } else if (grid_.getCellState(x, y) == Grid::GOAL) {
        // Goal cell
        cellColor = sf::Color::Red;
    } else if (grid_.getCellState(x, y) == Grid::PATH) {
        // Path cell
        cellColor = sf::Color::Green;
    } else if (grid_.getCellState(x, y) == Grid::EMPTY) {
            cellColor = sf::Color::White;
    } else if (grid_.getCellState(x, y) == Grid::VISITED) {
            cellColor = sf::Color::Magenta;
    } else {
        // Default to white for any other state
        cellColor = sf::Color::White;
    }
}

sf::RenderWindow& Visualizer::getWindow() { 
    return window_;
}
