#include "grid.hpp"
#include "obstacle.hpp"
#include "visualization.hpp"
#include <iostream>

using namespace std;

int main() {
    Grid grid(256, 256);
    Obstacle obstacle;
    Visualizer visualizer(grid, obstacle);

    if (visualizer.windowIsOpen()) {
        visualizer.displayGrid();
    }
    return 0;
}