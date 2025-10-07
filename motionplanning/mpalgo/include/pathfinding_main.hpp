#ifndef PATHFINDING_MAIN_HPP
#define PATHFINDING_MAIN_HPP

#include <vector>
#include <utility>
#include <string>
#include "grid.hpp"

using namespace std;

class PathFindingAlgorithm {
    private:
        vector<string> algorithms = {"A*", "Dijkstra", "BFS", "DFS", "RRT"}; // List of supported algorithms
        string selectedAlgo_ = "Dijkstra"; // Default algorithm
    protected:
        Grid& grid_;
        pair<int, int> startPos_; // Start position
        pair<int, int> goalPos_; // Goal position
        vector<pair<int, int>> foundPath_; // Store the found path
        vector<pair<int, int>> visitedNodes_; // Store visited nodes
    public:
        // Constructor and Destructor
        PathFindingAlgorithm(Grid& grid, const pair<int, int>& startPos, const pair<int, int>& goalPos) : grid_(grid), startPos_(startPos), goalPos_(goalPos) {}
        ~PathFindingAlgorithm() {}

        // Handle algorithm selection from dropdown by calling corresponding algorithm (e.g. A*, Dijkstra, BFS,...)
        void algoSelection(string& selectedAlgo_);

        // Provide found path from selected algorithm
        vector<pair<int, int>> getFoundPath() const {
            return foundPath_;
        };

        // Provide visited nodes from selected algorithm
        vector<pair<int, int>> getVisitedNodes() const {
            return visitedNodes_;
        };

        // Get neibhbors of a given cell (x, y)
        vector<pair<int, int>> getNeighbors(int x, int y) const;
};

#endif // PATHFINDING_MAIN_HPP