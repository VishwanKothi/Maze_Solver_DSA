#include <bits/stdc++.h>
#include <chrono>
using namespace std;

// Function to find the root of a node in the union-find data structure
pair<int, int> findRoot(pair<int, int> node, vector<vector<pair<int, int>>> &S) {
    pair<int, int> currentNode = node;
    while (S[currentNode.first][currentNode.second] != currentNode) {
        currentNode = S[currentNode.first][currentNode.second];
    }
    while (node != currentNode) {
        pair<int, int> parentNode = S[node.first][node.second];
        S[node.first][node.second] = currentNode;
        node = parentNode;
    }
    return node;
}

// Function to union two nodes in the union-find data structure
void unionNodes(pair<int, int> nodeA, pair<int, int> nodeB, vector<vector<pair<int, int>>> &S) {
    pair<int, int> r = findRoot(nodeA, S);
    S[r.first][r.second] = findRoot(nodeB, S);
}

// Custom hash function for pairs of pairs
namespace std {
    template <>
    struct hash<std::pair<std::pair<int, int>, std::pair<int, int>>> {
        size_t operator()(const std::pair<std::pair<int, int>, std::pair<int, int>>& Pair) const {
            size_t hash1 = std::hash<int>()(Pair.first.first);
            size_t hash2 = std::hash<int>()(Pair.first.second);
            size_t hash3 = std::hash<int>()(Pair.second.first);
            size_t hash4 = std::hash<int>()(Pair.second.second);
            return hash1 ^ (hash2 << 1) ^ (hash3 << 2) ^ (hash4 << 3);
        }
    };
}

// Global variables to track algorithm performance
int dfsCount = 0, bfsCount = 0, aStarCount = 0;
int dfsDuration = 0, bfsDuration = 0, aStarDuration = 0, mazeCreationDuration = 0;

// Function to perform depth-first search on a maze
unordered_set<pair<pair<int, int>, pair<int, int>>> depthFirstSearchPath(unordered_set<pair<pair<int, int>, pair<int, int>>> Maze, int n) {
    vector<vector<bool>> visited(n, vector<bool>(n, false));
    vector<vector<pair<int, int>>> parent(n, vector<pair<int, int>>(n, {-1, -1}));
    vector<pair<int, int>> stack;
    stack.push_back({0, 0});
    visited[0][0] = true;
    while (!stack.empty()) {
        pair<int, int> currentNode = stack.back();
        stack.pop_back();
        int x = currentNode.first, y = currentNode.second;
        visited[x][y] = true;
        dfsCount++;
        if (currentNode == make_pair(n - 1, n - 1)) break;
        if (x < n - 1 && Maze.find({{x + 1, y}, {x + 1, y + 1}}) == Maze.end() && !visited[x + 1][y]) {
            stack.push_back({x + 1, y});
            parent[x + 1][y] = {x, y};
        }
        if (x > 0 && Maze.find({{x, y}, {x, y + 1}}) == Maze.end() && !visited[x - 1][y]) {
            stack.push_back({x - 1, y});
            parent[x - 1][y] = {x, y};
        }
        if (y < n - 1 && Maze.find({{x, y + 1}, {x + 1, y + 1}}) == Maze.end() && !visited[x][y + 1]) {
            stack.push_back({x, y + 1});
            parent[x][y + 1] = {x, y};
        }
        if (y > 0 && Maze.find({{x, y}, {x + 1, y}}) == Maze.end() && !visited[x][y - 1]) {
            stack.push_back({x, y - 1});
            parent[x][y - 1] = {x, y};
        }
    }
    pair<int, int> endNode = {n - 1, n - 1};
    unordered_set<pair<pair<int, int>, pair<int, int>>> path;
    while (parent[endNode.first][endNode.second] != make_pair(-1, -1)) {
        pair<int, int> ancestor = parent[endNode.first][endNode.second];
        if (endNode.first == ancestor.first + 1 || endNode.second == ancestor.second + 1) {
            path.insert({ancestor, endNode});
        } else {
            path.insert({endNode, ancestor});
        }
        endNode = ancestor;
    }
    return path;
}

// Custom comparator for priority queue in A* search
struct NodeComparator {
    int n;
    NodeComparator(int n) : n(n) {}
    bool operator()(pair<int, int> nodeA, pair<int, int> nodeB) {
        int costA = 2 * (n - 1) - (nodeA.first + nodeA.second);
        int costB = 2 * (n - 1) - (nodeB.first + nodeB.second);
        if (costA < costB) {
            return nodeB < nodeA;
        } else {
            return nodeA > nodeB;
        }
    }
};

// Function to perform A* search on a maze
unordered_set<pair<pair<int, int>, pair<int, int>>> aStarSearchPath(unordered_set<pair<pair<int, int>, pair<int, int>>> Maze, int n) {
    vector<vector<bool>> visited(n, vector<bool>(n, false));
    vector<vector<pair<int, int>>> parent(n, vector<pair<int, int>>(n, {-1, -1}));
    NodeComparator customComparator(n);
    priority_queue<pair<int, int>, vector<pair<int, int>>, NodeComparator> stack(customComparator);
    stack.push({0, 0});
    visited[0][0] = true;
    while (!stack.empty()) {
        pair<int, int> currentNode = stack.top();
        stack.pop();
        int x = currentNode.first, y = currentNode.second;
        visited[x][y] = true;
        aStarCount++;
        if (currentNode == make_pair(n - 1, n - 1)) break;
        if (x < n - 1 && Maze.find({{x + 1, y}, {x + 1, y + 1}}) == Maze.end() && !visited[x + 1][y]) {
            stack.push({x + 1, y});
            parent[x + 1][y] = {x, y};
        }
        if (x > 0 && Maze.find({{x, y}, {x, y + 1}}) == Maze.end() && !visited[x - 1][y]) {
            stack.push({x - 1, y});
            parent[x - 1][y] = {x, y};
        }
        if (y < n - 1 && Maze.find({{x, y + 1}, {x + 1, y + 1}}) == Maze.end() && !visited[x][y + 1]) {
            stack.push({x, y + 1});
            parent[x][y + 1] = {x, y};
        }
        if (y > 0 && Maze.find({{x, y}, {x + 1, y}}) == Maze.end() && !visited[x][y - 1]) {
            stack.push({x, y - 1});
            parent[x][y - 1] = {x, y};
        }
    }
    pair<int, int> endNode = {n - 1, n - 1};
    unordered_set<pair<pair<int, int>, pair<int, int>>> path;
    while (parent[endNode.first][endNode.second] != make_pair(-1, -1)) {
        pair<int, int> ancestor = parent[endNode.first][endNode.second];
        if (endNode.first == ancestor.first + 1 || endNode.second == ancestor.second + 1) {
            path.insert({ancestor, endNode});
        } else {
            path.insert({endNode, ancestor});
        }
        endNode = ancestor;
    }
    return path;
}
