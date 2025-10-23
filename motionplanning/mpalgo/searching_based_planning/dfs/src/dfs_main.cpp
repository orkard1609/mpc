#include <vector>
#include <utility>
#include <string>
#include <iostream>
#include <algorithm>
#include "dfs.hpp"
#include "pathfinding_main.hpp"

using namespace std;

void DFS::dfs(pair<int, int> cur, pair<int, int> goal, 
              vector<vector<int>>& dist, vector<vector<pair<int, int>>>& parent) {
                vector<pair<int, int>> neighbors;
                neighbors = getNeighbors(cur.first, cur.second);
                // Stop when reaching the goal
                if (cur == goal) return;
                // Check all possible directions
                for (int i = 0; i < neighbors.size(); i++) {
                    int nx = neighbors[i].first;
                    int ny = neighbors[i].second;
                    if (dist[nx][ny] == -1) {
                        dist[nx][ny] = dist[cur.first][cur.second] + 1;     // update distance
                        parent[nx][ny] = cur;                               // store parent
                        dfs(pair<int, int>{nx, ny}, goal, dist, parent);    // recursive call to keep exploring current neighbor
                        visitedNodes_.push_back({nx, ny});                  // store for visited node visualization
                    }
                }
}

vector<pair<int, int>> DFS::findPath(const vector<vector<int>>& grid, pair<int, int> start, pair<int, int> goal) {
    vector<pair<int, int>> path;
    //vector<pair<int, int>> neighbors;
    int n = grid.size(), m = grid[0].size();

    vector<vector<int>> dist(n, vector<int>(m, -1)); // Distance array from start to each cell
    vector<vector<pair<int, int>>> parent(n, vector<pair<int, int>>(m, {-1,-1})); // Store parent to reconstruct path

    // Push start point into stack
    dist[start.first][start.second] = 0;

    //neighbors = getNeighbors(start.first, start.second);
    
    pair<int, int> cur = start;

    dfs(cur, goal, dist, parent);

    // If goal is unreachable then there is not path
    if (dist[goal.first][goal.second] == -1) {
        return vector<pair<int, int>>(); // return empty path
    }

    // Trace back the path from goal to start using parent array
    for (pair<int, int> p = goal; p.first != -1; p = parent[p.first][p.second]) {
        // Remove start and goal cells out of found path
        if (p != start && p != goal) {
            path.push_back(p);
        }
    }
    reverse(path.begin(), path.end()); // Reverse the path to get it from start to goal

    return path;
}