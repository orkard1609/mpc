#include <vector>
#include <utility>
#include "grid.hpp"
#include "obstacle.hpp"

using namespace std;

//Visualizer class to handle visualization related stuffs
class Visualizer {
    private:
        //Coloring empty cells in white and obstacles in black
        Grid grid;
        Obstacle obstacleColor;
    public:
        //Constructor to initialize visualizer with grid and obstacle
        Visualizer(int width, int height) : gridColor(width, height), obstacleColor(0, 0) {}
        //Destructor
        ~Visualizer() {}
        //Display empty cells in white
        void displayGrid() const {};

    protected:
        void getCellClick(int& x, int& y) const {
            // Get the cell click position from the user
        }
        void coloringCell(int x, int y, const string& color) const {
            // Color the cell at (x, y) with the specified color
        }

    private:
        void undoClick() const {
            // Handle undo click event
        }
        void resizeClick(int newWidth, int newHeight) const {
            // Handle resize click event
        }
};