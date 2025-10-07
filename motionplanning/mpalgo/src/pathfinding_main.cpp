#include <vector>
#include <utility>
#include <string>
#include "grid.hpp"
#include "pathfinding_main.hpp"

using namespace std;

void PathFindingAlgorithm::algoSelection(string& selectedAlgo_) {
    foundPath_.clear();
    visitedNodes_.clear();
    if (selectedAlgo_ == "A*") {
        // aStar();
    } else if (selectedAlgo_ == "Dijkstra") {
        // Dijkstra();
    } else if (selectedAlgo_ == "BFS") {
        // BFS();
    } else if (selectedAlgo_ == "DFS") {
        // DFS();
    } else if (selectedAlgo_ == "RRT") {
        // RRT();
    }
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