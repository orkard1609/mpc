//Visualization for grid map creation and path planning
#include <iostream>
#include <vector>
#include "visualization.hpp"

using namespace std;

class gridCreation {
    private:
        int width;
        int height;
        vector<int> grid;
    public:
    //Constructor to initialize grid with given width and height
    gridCreation(int w, int h) : width(w), height(h) {}

    //Get grid width;
    int getWidth(int w) const {
        return width;
    }

    //Get grid height;
    int getHeight(int h) const {
        return height;
    }

    //Display grid
    void displayGrid() const;

    //Get obstacle positions
    vector<pair<int, int>> getObstaclePositions() const;

    //Set obstacle at given position
    void setObstacle(int x, int y);
};

void gridCreation::displayGrid() const {
}

vector<pair<int, int>> gridCreation::getObstaclePositions() const {
}

void gridCreation::setObstacle(int x, int y) {
}