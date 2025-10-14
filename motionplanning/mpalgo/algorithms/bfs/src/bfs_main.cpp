#include <vector>
#include <utility>
#include <string>
#include "bfs.hpp"

using namespace std;

vector<pair<int, int>> BFS::findPath(const vector<vector<int>>& grid, pair<int, int> start, pair<int, int> goal) {
    // Implement BFS algorithm here
    vector<pair<int, int>> path;

    struct Point { int x, y; };

    vector<pair<int, int>> neighbors = getNeighbors(start.first, start.second);

    int n = grid.size(), m = grid[0].size();

    queue<Point> q;  // Queue for BFS
    vector<vector<int>> dist(n, vector<int>(m, -1)); // Distance array from start to each cell
    vector<vector<Point>> parent(n, vector<Point>(m, {-1,-1})); // Store parent to reconstruct path

    // Push start point into queue
    q.push(start);
    dist[start.x][start.y] = 0;

    // BFS loop
    while (!q.empty()) {
        auto cur = q.front(); 
        q.pop();

        // Stop when reaching the goal
        if (cur.x == goal.x && cur.y == goal.y) break;

        // Check 4 possible directions
        for (int i = 0; i < 4; i++) {
            int nx = cur.x + dx[i];
            int ny = cur.y + dy[i];

            // Check if the new position is valid
            if (nx>=0 && ny>=0 && nx<n && ny<m && grid[nx][ny]==0 && dist[nx][ny]==-1) {
                dist[nx][ny] = dist[cur.x][cur.y] + 1; // update distance
                parent[nx][ny] = cur;                  // store parent
                q.push({nx, ny});                      // push new position into queue
            }
        }
    }

    // If goal is unreachable then there is not path
    if (dist[goal.x][goal.y] == -1) {
        cout << "No path\n";
        return 0;
    }

    // Trace back the path from goal to start using parent array
    vector<Point> path;
    for (Point p = goal; p.x != -1; p = parent[p.x][p.y])
        path.push_back(p);
    reverse(path.begin(), path.end()); // reverse to get path from start to goal

    cout << "Shortest path length = " << dist[goal.x][goal.y] << endl;
    cout << "Path: ";
    for (auto p : path)
        cout << "(" << p.x << "," << p.y << ") ";
    cout << endl;

    // BFS logic to find the path from start to goal
    return path;
}