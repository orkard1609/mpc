#include <vector>
#include <utility>
#include "grid.hpp"

using namespace std;

//Obstacle class to handle obstacle related stuffs
class Obstacle {
    private:
        //obstaclePos: vector to store obstacle positions
        vector<pair<int, int>> obstaclePositions;
    public:
        //Constructor to initialize obstacle
        Obstacle(vector<pair<int, int>> positions) : obstaclePositions(positions) {}
        //Destructor
        ~Obstacle() {}
        //Get obstacle positions, get triggered when user clicks on "Set Obstacle" button 
        vector<pair<int, int>> getObstaclePositions() const { return obstaclePositions; }
        //Undo obstacle, get triggered when user clicks on "Undo" button, coloring the cells in white
        void undoObstacle(Grid& grid);
        //Set obstacle at given position, get triggered when user clicks on "Confirm Obstacle" button, coloring the cells in black
        void setObstacle(vector<pair<int, int>> position) {}
};