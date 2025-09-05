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

    sf::Vector2i mousePos;
    vector<pair<int, int>> clickedCells;
    string clickedButtons;
    while (visualizer.windowIsOpen()) {

        mousePos = sf::Mouse::getPosition(visualizer.getWindow());

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

            // Check if a button is clicked by left mouse button
            if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    //cout << "Mouse Position: " << mousePos.x << ", " << mousePos.y << endl;
                    visualizer.setMousePos(mousePos.x, mousePos.y);
                    clickedCells = visualizer.getCellClick();
                    clickedButtons = visualizer.getButtonClick();
                    if (!clickedCells.empty()) {
                        cout << "Clicked Cell: (" << clickedCells[0].first << ", " << clickedCells[0].second << ")" << endl;
                    }   
                    if (!clickedButtons.empty()) {
                        cout << "Clicked Button: " << clickedButtons << endl;
                    }
                }
            }
        }

        visualizer.displayWindows();
    }

    cout << "MotionPlanning finished." << endl;
    return 0;
} 