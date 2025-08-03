#include <vector>
#include <utility>
#include "grid.hpp"
#include "obstacle.hpp"

using namespace std;

//Visualizer class to handle visualization related stuffs
class Visualizer {
    private:
        //Coloring empty cells in white and obstacles in black
        Grid gridColor;
        Obstacle obstacleColor;
    public:
        //Constructor to initialize visualizer with grid and obstacle
        Visualizer(int width, int height) : gridColor(width, height), obstacleColor(0, 0) {}
        //Destructor
        ~Visualizer() {}

        //Display empty cells in white
        void displayGrid() const {};
        //Display obstacles in black
        void displayObstacles() const {};
        //Display path in green
        void displayPath() const {};
        //Handle user input
        void handleUserInput() const {};
        //Display status, check if the grid is open for interaction
        bool gridisOpen() const { return true; }
};