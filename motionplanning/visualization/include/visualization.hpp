#include <vector>
#include <utility>
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
        int const cellSize_ = 10; // Size of each cell in pixels
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
        void getCellClick(int& x, int& y) {
            // Get the cell click position from the user
        }
        // Handle undo click event
        void undoClick() {
        // 
        }
        // Handle resize click event
        void resizeClick(int newWidth, int newHeight) {
        // 
        }

    protected:
        // Get the cell click position from the user
        void coloringCell(int x, int y, const string& color) const {
            // Color the cell at (x, y) with the specified color
        }


};