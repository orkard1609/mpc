#include <vector>
#include <utility>
#include <string>
#include <iostream>
#include <memory>  // For unique_ptr and make_unique
#include "grid.hpp"
#include "pathfinding_main.hpp"
#include "bfs.hpp"

using namespace std;

unique_ptr<PathFindingAlgorithm> PathFindingAlgorithm::selectAlgorithm(const string& algo, Grid& grid, 
                                                                            const pair<int, int>& start, 
                                                                            const pair<int, int>& goal) {
    if (algo == "BFS") {
        return make_unique<BFS>(grid, start, goal, algo);
    } else if (algo == "DFS") {
        //return make_unique<DFS>(grid, start, goal, algo);
        cout << "DFS algorithm is not supported yet." << endl;
    } else if (algo == "A*") {
        //return make_unique<AStar>(grid, start, goal, algo);
        cout << "A* algorithm is not supported yet." << endl;
    } else if (algo == "Dijkstra") {
        //return make_unique<Dijkstra>(grid, start, goal, algo);
        cout << "Dijkstra algorithm is not supported yet." << endl;
    } else if (algo == "RRT") {
        //return make_unique<RRT>(grid, start, goal, algo);
        cout << "RRT algorithm is not supported yet." << endl;
    } else {
        // Fallback for unrecognized algorithm
        cout << "Algorithm " << algo << " is not recognized." << endl;
    }
    // Return nullptr for unsupported or unrecognized algorithms
    return nullptr;
}

vector<pair<int, int>> PathFindingAlgorithm::getNeighbors(int x, int y) const {
    vector<pair<int, int>> neighbors;
    vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // Up, Down, Left, Right
                                        //{ -1, -1}, { -1, 1}, { 1, -1}, { 1, 1}}; // Diagonal cells for further use

    for (const auto& dir : directions) {
        int newX = x + dir.first;
        int newY = y + dir.second;
        // Check if the new position is within grid bounds and not an obstacle
        if (newX >= 0 && newX < grid_.getWidth() && newY >= 0 && newY < grid_.getHeight() && 
            grid_.getCellState(newX, newY) != Grid::OBSTACLE) {
            neighbors.push_back({newX, newY});
        }
    }
    return neighbors;
}