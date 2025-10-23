#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <queue>
#include <algorithm>
#include "bfs.hpp"

using namespace std;

vector<pair<int, int>> BFS::findPath(const vector<vector<int>>& grid, pair<int, int> start, pair<int, int> goal) {
    vector<pair<int, int>> path;
    vector<pair<int, int>> neighbors;
    int n = grid.size(), m = grid[0].size();

    queue<pair<int, int>> q;  // Queue for BFS
    vector<vector<int>> dist(n, vector<int>(m, -1)); // Distance array from start to each cell
    vector<vector<pair<int, int>>> parent(n, vector<pair<int, int>>(m, {-1,-1})); // Store parent to reconstruct path

    // Push start point into queue
    q.push(start);
    dist[start.first][start.second] = 0;

    // BFS loop
    while (!q.empty()) {
        auto cur = q.front(); 
        q.pop();
        // Stop when reaching the goal
        if (cur == goal) break;
        // Get neighbors
        neighbors = getNeighbors(cur.first, cur.second);
        // Check all possible directions
        for (int i = 0; i < neighbors.size(); i++) {
            int nx = neighbors[i].first;
            int ny = neighbors[i].second;
            // Check if the neighbor is not visited
            if (dist[nx][ny] == -1) {
                dist[nx][ny] = dist[cur.first][cur.second] + 1; // update distance
                parent[nx][ny] = cur;                           // store parent
                q.push({nx, ny});                               // push new position into queue
                visitedNodes_.push_back({nx, ny});              // store for visited node visualization
            }
        }
    }
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
    reverse(path.begin(), path.end()); // reverse to get path from start to goal
    
    return path;
}