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
        visualizer.displayWindows();
    }

    cout << "MotionPlanning finished." << endl;
    return 0;
} 