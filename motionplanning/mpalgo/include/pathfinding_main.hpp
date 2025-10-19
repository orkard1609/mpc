#ifndef PATHFINDING_MAIN_HPP
#define PATHFINDING_MAIN_HPP

#include <vector>
#include <utility>
#include <string>
#include <iostream>
#include <memory>
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
        // Constructor
        PathFindingAlgorithm(Grid& grid, const pair<int, int>& startPos, const pair<int, int>& goalPos, string selectedAlgo) 
                            : grid_(grid), startPos_(startPos), goalPos_(goalPos), selectedAlgo_(selectedAlgo) {}
        // Destructor for virtual base class
        virtual ~PathFindingAlgorithm() {}

        // Static factory method to create algorithm instances
        static unique_ptr<PathFindingAlgorithm> selectAlgorithm(const string& algo, Grid& grid, 
                                                                const pair<int, int>& start, 
                                                                const pair<int, int>& goal);

        // Main interfaces method, provide found path from selected algorithm
        vector<pair<int, int>> getFoundPath() {
            if (foundPath_.empty()) {
                foundPath_ = findPath(grid_.getGridStatus(), startPos_, goalPos_);
            }
            return foundPath_;
        }

        // Helper methods, provide visited nodes from selected algorithm
        vector<pair<int, int>> getVisitedNodes() const {
            return visitedNodes_;
        };

        // Helper methods, get neighbors of a given cell (x, y)
        vector<pair<int, int>> getNeighbors(int x, int y) const;

        // Call path finding algorithms - virtual function to be overridden by derived classes
        virtual vector<pair<int, int>> findPath(const vector<vector<int>>& grid, 
                                                pair<int, int> start, 
                                                pair<int, int> goal) = 0;
};

#endif // PATHFINDING_MAIN_HPP