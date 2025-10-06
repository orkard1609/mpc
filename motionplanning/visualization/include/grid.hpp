#ifndef GRID_CLASS
#define GRID_CLASS

#include <vector>
#include <utility>

using namespace std;

// Forward declaration to avoid circular dependency
class Obstacle;

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
        vector<vector<int>> gridStatus; // 2D vector to represent the grid
    public:
        // Enum for cell status
        enum cellStatus { EMPTY, OBSTACLE, START, GOAL, PATH }; 

        //Constructor to initialize grid with given width and height
        Grid(int width = 256, int height = 256);
        //Destructor
        ~Grid() {}
        //Get grid width;
        int getWidth() const { return width;}
        //Get grid height;
        int getHeight() const { return height; }
        //Get triggered when user clicks on "Set Grid Size" button
        void gridResize(int newWidth, int newHeight);
        //Validity check for grid size. Check if the given width and height are valid
        bool isValid(int width, int height) const { return (width > 0 && height > 0);}
        //Set cell state based on obstacle, start and goal positions
        void setCellState(int x, int y, cellStatus status);
        //Get cell state
        Grid::cellStatus getCellState(int x, int y) const;
};

#endif // GRID_CLASS