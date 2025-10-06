#ifndef VISUALIZATION_CLASS
#define VISUALIZATION_CLASS

#include <vector>
#include <utility>
#include <string>
#include <map>
#include "grid.hpp"
#include "obstacle.hpp"
#include <SFML/Graphics.hpp>

using namespace std;

// Struct to define interactive boxes and texts
struct controlButton {
    int x; // Box start positions
    int y;
    int width; // Box size 
    int height; 
    
    string boxLabel; // Box label
    string boxType; // Including "interactiveBox", "textOnly" and "inputBox"
    string boxID;  // Unique identifier for each box/button

    // Check if the box is clicked
    bool isBoxClicked(int mouseX, int mouseY) const {
        return (mouseX >= x && mouseX <= x + width && 
                mouseY >= y && mouseY <= y + height);
    }
};

//Visualizer class to handle visualization related stuffs
class Visualizer {
    private:
        sf::RenderWindow window_; // SFML window for rendering
        Grid& grid_;
        sf::Color cellColor; // Color corresponding to cell status
        Obstacle& obstacle_;
        vector<pair<int, int>> obstacleStack_; // Store obstacle positions
        string clickedButton; // Global clicked button state
        vector<pair<int, int>> clickedCell; // Global clicked cell state
        int cellSize_ = 30; // Size of each cell in pixels
        unsigned int gridWidth_, gridHeight_;  // Store grid width and height in pixels
        int gridOffsetX_ = 0; // X offset for grid positioning
        int gridOffsetY_ = 0; // Y offset for grid positioning
        int boxAlignmentX_ = 0; // X position for control panel alignment
        int boxAlignmentY_ = 0; // Y position for control panel alignment
        unsigned int windowWidth_ = 0; // Width of the entire window
        unsigned int windowHeight_ = 0; // Height of the entire window
        unsigned int controlPanelWidth_ = 200; // Width of the control panel area
        int mousePos_x = 0; // Current mouse position, x
        int mousePos_y = 0; // Current mouse position, y
        map<string, controlButton> buttons_; // Map to store buttons by ID
        sf::Font font; // Font of text
        sf::Clock cursorBlinkClock_; // Timer for cursor blinking
        bool cursorVisible_ = true; // Current state of cursor visibility
        string inputBoxTextX_ = ""; // Text currently in the active input box X
        string inputBoxTextY_ = ""; // Text currently in the active input box Y
        vector<string> algorithms = {"A*", "Dijkstra", "BFS", "DFS", "RRT"}; // <= Will be moved to algorithm class
        string selectedAlgo_ = "Dijkstra"; // Init algorithm from dropdown;
        bool isAlgoDropdownOpen_ = false; // Track if algorithm dropdown is open
        bool clickedOutside_ = false; // Track if user clicked outside dropdown list
        string clickedAlgo_ = ""; // Store the algorithm selected from dropdown list
        bool isObstacleSet_ = false; // Track if user is in obstacle setting mode
        vector<pair<int, int>> selectedStartGoal_; // Store current start/goal positions being set
        bool isStartGoalSet_ = false; // Track if user is in start-goal position setting mode
    public:
        // Constructor to initialize visualizer with grid and obstacle
        Visualizer(Grid& grid, Obstacle& obstacle);
        // Destructor
        ~Visualizer() {}

        // Display windows, including grid and interactive elements
        void displayWindows();
        // Check if the window is open
        bool windowIsOpen() const {
            return window_.isOpen();
        }
        // Set mouse position
        void setMousePos(int x, int y) {
            mousePos_x = x;
            mousePos_y = y;
        }

        // Get the cell click position from the user, return clicked cell coordinates
        vector<pair<int, int>> getCellClick();

        // Check if a button is clicked by left mouse button, return clicked button
        string getButtonClick();

        // Handle resize click event, including resize grid and confirm resize
        void handleGridResize(char inputChar = 0); // Default value of 0 means no character input

        // Handle set obstacle click event, including set obstacle, undo selected obstacle and confirm obstacle
        void handleSetObstacle();

        // Handle start/goal selection click event, including set start/goal and confirm selection
        void handleStartGoalSelection();

        // Handle path planning box click event, confirm selected algo in drop down list
        void handlePathPlanningBox();

        // Handle path planning algorithm, getting found path from MPAlgo class
        void handlePathPlanningAlgo();

        // Draw all control buttons
        void drawControlButton(int x, int y, int width, int height, const std::string& inputText, const std::string& boxType);
        
        // Handle all events
        void handleEvents();

        // Reset grid and related variables to initial state
        void resetWindows();

        // Get the SFML window
        sf::RenderWindow& getWindow();

    protected:
        // Color the cell based on its state (empty, obstacle, start, goal, path)
        void coloringCell(int x, int y, sf::Color& cellColor) const;
};

#endif // VISUALIZATION_CLASS