#include "grid.hpp"
#include "obstacle.hpp"
#include "visualization.hpp"
#include <iostream>
#include <optional>  // Required for std::optional

using namespace std;

int main() {
    cout << "Starting Motion Planning Visualizer..." << endl;
    Grid grid(20, 20);
    Obstacle obstacle;
    Visualizer visualizer(grid, obstacle);

    while (visualizer.windowIsOpen()) {
        visualizer.handleEvents();    // Handle all events first
        visualizer.displayWindows();  // Then update display
    }

    cout << "MotionPlanning finished." << endl;
    return 0;
}