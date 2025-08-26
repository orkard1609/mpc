#include "grid.hpp"
#include "obstacle.hpp"
#include "visualization.hpp"
#include <iostream>
#include <optional>  // Required for std::optional

using namespace std;

/* int main() {
    cout << "Starting MotionPlanning..." << endl;
    Grid grid(20, 20);
    Obstacle obstacle;
    Visualizer visualizer(grid, obstacle);

    while (visualizer.windowIsOpen()) {
        // SFML 3.x returns std::optional<sf::Event>
        while (auto optionalEvent = visualizer.getWindow().pollEvent()) {
            const sf::Event& event = *optionalEvent; // Dereference to get the event
            // SFML 3.x uses template-based event checking
            if (event.is<sf::Event::Closed>()) { // Access type member
                visualizer.getWindow().close();
            }
            
            // Example of accessing event data in SFML 3.x
            if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Escape) {
                    visualizer.getWindow().close();
                }
            }
        }
        visualizer.displayGrid();
    }

    cout << "MotionPlanning finished." << endl;
    return 0;
} */

int main() {
    // Create grid and obstacle instances
    Grid grid(20, 15); // 20x15 grid
    Obstacle obstacle;
    
    // Create visualizer with grid and obstacle
    Visualizer visualizer(grid, obstacle);
    
    // Display the initial grid
    visualizer.displayGrid();
    
    // Main application loop
    while (visualizer.windowIsOpen()) {
        sf::Event event;
        sf::RenderWindow& window = visualizer.getWindow();
        
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // Handle mouse click events
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                int x, y;
                visualizer.getCellClick(x, y);
                cout << "Cell clicked: (" << x << ", " << y << ")" << endl;
                
                // Toggle cell: if it's an obstacle, make it empty, otherwise make it an obstacle
                if (grid.isObstacle(x, y)) {
                    // Note: This is just for visualization purposes. In a real application,
                    // you would need to properly update the obstacle data structure
                    visualizer.coloringCell(x, y, "white");
                } else {
                    visualizer.coloringCell(x, y, "black");
                }
            }
            // Handle key press events
            else if (event.type == sf::Event::KeyPressed) {
                // Undo last obstacle placement when 'U' is pressed
                if (event.key.code == sf::Keyboard::U) {
                    visualizer.undoClick();
                    cout << "Undoing last obstacle placement" << endl;
                }
                // Reset the grid when 'R' is pressed
                else if (event.key.code == sf::Keyboard::R) {
                    int newWidth = 20, newHeight = 15;  // Default sizes
                    visualizer.resizeClick(newWidth, newHeight);
                    visualizer.displayGrid();
                    cout << "Grid resized to " << newWidth << "x" << newHeight << endl;
                }
            }
        }
    }
    
    return 0;
}