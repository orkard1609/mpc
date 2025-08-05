#include <vector>
#include <utility>
#include "grid.hpp"

using namespace std;

//Obstacle class to handle obstacle related stuffs
class Obstacle {
    private:
        //obstaclePos: vector to store obstacle positions, LIFO stack to store the positions of obstacles
        vector<pair<int, int>> obstaclePositions;
    public:
        //Constructor to initialize obstacle
        Obstacle(vector<pair<int, int>> positions) : obstaclePositions(positions) {}
        //Destructor
        ~Obstacle() {}
        //Get obstacle positions, get triggered when user clicks on "Set Obstacle" button 
        vector<pair<int, int>> addObstacle(vector<pair<int, int>>& obstaclePositions, int x, int y);
        //Undo obstacle, get triggered when user clicks on "Undo" button, coloring the cells in white
        vector<pair<int, int>> undoObstacle(vector<pair<int, int>>& obstaclePositions);
        //Set obstacle at given position, get triggered when user clicks on "Confirm Obstacle" button, coloring the cells in black
        vector<pair<int, int>> getObstacle() const { return obstaclePositions; }
};