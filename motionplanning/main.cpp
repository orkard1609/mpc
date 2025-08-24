/*#include "grid.hpp"
#include "obstacle.hpp"
#include "visualization.hpp"
#include <iostream>

using namespace std;

int main() {
    cout << "Starting MotionPlanning..." << endl;
    Grid grid(256, 256);
    Obstacle obstacle;
    Visualizer visualizer(grid, obstacle);

    while (visualizer.windowIsOpen()) { // Loop while the window is open
        sf::Event event;
        while (visualizer.getWindow().pollEvent(event)) { // Access window via a getter
            if (event.type == sf::Event::Closed) {
                visualizer.getWindow().close(); // Close window on close event
            }
        }
        visualizer.displayGrid(); // Continuously render the grid
    }

    cout << "MotionPlanning finished." << endl;
    return 0;
}*/

/*#include "grid.hpp"
#include "obstacle.hpp"
#include "visualization.hpp"
#include <iostream>

using namespace std;

#include "grid.hpp"
#include "obstacle.hpp"
#include "visualization.hpp"
#include <iostream>
#include <optional>  // For std::optional

using namespace std;

int main() {
    cout << "Starting MotionPlanning..." << endl;
    Grid grid(256, 256);
    Obstacle obstacle;
    Visualizer visualizer(grid, obstacle);

    while (visualizer.windowIsOpen()) {
        auto optionalEvent = visualizer.getWindow().pollEvent();  // SFML 3 returns optional<sf::Event>
        if (optionalEvent) {
            const sf::Event& event = *optionalEvent;  // Access the event
            if (event.type == sf::Event::Closed) {
                visualizer.getWindow().close();
            }
        }
        visualizer.displayGrid();
    }

    cout << "MotionPlanning finished." << endl;
    return 0;
}*/

#include "grid.hpp"
#include "obstacle.hpp"
#include "visualization.hpp"
#include <iostream>
#include <optional>  // Required for std::optional

using namespace std;

int main() {
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
}