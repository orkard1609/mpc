#ifndef BFS_HPP
#define BFS_HPP

#include <vector>
#include <utility>
#include <string>
#include "pathfinding_main.hpp"

using namespace std;

class BFS : public PathFindingAlgorithm {
    public:
        // Inherit constructor from the base class
        using PathFindingAlgorithm::PathFindingAlgorithm;
        
        // Override the findPath method of the base class
        vector<pair<int, int>> findPath(const vector<vector<int>>& grid, pair<int, int> start, pair<int, int> goal) override;
};

#endif // BFS_HPP