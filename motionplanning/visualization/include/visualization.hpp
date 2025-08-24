#ifndef VISUALIZATION_CLASS
#define VISUALIZATION_CLASS

#include <vector>
#include <utility>
#include <string>
#include "grid.hpp"
#include "obstacle.hpp"
#include <SFML/Graphics.hpp>

using namespace std;

//Visualizer class to handle visualization related stuffs
class Visualizer {
    private:
        sf::RenderWindow window_; // SFML window for rendering
        Grid& grid_;
        Obstacle& obstacle_;
        int cellSize_ = 30; // Size of each cell in pixels
    public:
        //Constructor to initialize visualizer with grid and obstacle
        Visualizer(Grid& grid, Obstacle& obstacle);
        //Destructor
        ~Visualizer() {}

        //Display empty cells in white
        void displayGrid();
        //Check if the window is open
        bool windowIsOpen() const {
            return window_.isOpen();
        }
        //Get the cell click position from the user
        void getCellClick(int& x, int& y);
        // Handle undo click event
        void undoClick();
        // Handle resize click event
        void resizeClick(int newWidth, int newHeight);
        // Get the SFML window
        sf::RenderWindow& getWindow();

    protected:
        // Get the cell click position from the user
        void coloringCell(int x, int y, const string& color) const;


};
#endif // VISUALIZATION_CLASS