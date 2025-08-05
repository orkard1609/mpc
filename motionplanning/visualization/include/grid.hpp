#include <vector>
#include <utility>
#include "obstacle.hpp"

using namespace std;

//Grid class to handle grid related stuffs
class Grid {
    private:
        //Width: number of cells in x axis
        int width;
        //Height: number of cells in y axis
        int height;
        //cellPos: vector to store cell positions, redundant variable, can be removed later
        vector<pair<int, int>> cellPos;
        //gridStatus: vector to store grid status
        vector<vector<int>> gridStatus;

    public:
        //Constructor to initialize grid with given width and height
        Grid(int width = 256, int height = 256) : width(width), height(height) {}
        //Destructor
        ~Grid() {}
        //Get grid width;
        int getWidth() const { return width;}
        //Get grid height;
        int getHeight() const { return height; }
        // Return the positions of the cells in the grid
        const vector<vector<int>>& getGrid() const { return gridStatus;}
        //Get triggered when user clicks on "Set Grid Size" button
        void gridResize(int newWidth, int newHeight) {}
        //Validity check for grid size. Check if the given width and height are valid
        bool isValid(int width, int height) const { return (width > 0 && height > 0);}
        // Set an obstacle at the specified position, get input from Obstacle class
        void setObstacle(int x, int y, Obstacle& obstacleStack) {}
        // Clear all obstacles in the grid, get triggered when user clicks on "Re-start" button
        void clearObstacle(int height, int width) {}
};