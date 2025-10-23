#ifndef DFS_HPP
#define DFS_HPP

#include <vector>
#include <utility>
#include <string>
#include "pathfinding_main.hpp"

using namespace std;

class DFS : public PathFindingAlgorithm {
    public:
        // Inherit constructor from the base class
        using PathFindingAlgorithm::PathFindingAlgorithm;

        // Override the findPath method of the base class
        vector<pair<int, int>> findPath(const vector<vector<int>>& grid, pair<int, int> start, pair<int, int> goal) override;

        void dfs(pair<int, int> cur, pair<int, int> goal, 
                 vector<vector<int>>& dist, vector<vector<pair<int, int>>>& parent);
};

#endif // DFS_HPP