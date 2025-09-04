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
        Obstacle& obstacle_;
        int cellSize_ = 30; // Size of each cell in pixels
        unsigned int gridWidth_, gridHeight_;  // Store grid width and height in pixels
        int gridOffsetX_ = 0; // X offset for grid positioning
        int gridOffsetY_ = 0; // Y offset for grid positioning
        map<string, controlButton> buttons_; // Map to store buttons by ID
        sf::Font font; // Font of text
        sf::Vector2i mousePos_; // Mouse position
    public:
        //Constructor to initialize visualizer with grid and obstacle
        Visualizer(Grid& grid, Obstacle& obstacle);
        //Destructor
        ~Visualizer() {}

        //Display windows, including grid and interactive elements
        void displayWindows();
        //Check if the window is open
        bool windowIsOpen() const {
            return window_.isOpen();
        }
        //Get the cell click position from the user
        void getCellClick(int& x, int& y);

        // Handle resize click event, including resize grid and confirm resize
        void handleGridResize(int newWidth, int newHeight);

        // Handle set obstacle click event, including set obstacle, undo selected obstacle and confirm obstacle
        void handleSetObstacle();

        // Handle start/goal selection click event, including set start/goal and confirm selection
        void handleStartGoalSelection();

        // Handle path planning box click event, confirm selected algo and start planning, reset the window after displayed the path
        void handlePathPlanningBox();

        // Handle path planning algorithm selection, including select path planning options in drop down list
        void handlePathPlanningAlgo();

        // Draw all control buttons
        void drawControlButton(int x, int y, int width, int height, const std::string& inputText, const std::string& boxType);
        
        // Get the SFML window
        sf::RenderWindow& getWindow();

    protected:
        // Get the cell click position from the user
        void coloringCell(int x, int y, const string& color) const;
};

#endif // VISUALIZATION_CLASS