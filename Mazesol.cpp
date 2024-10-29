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
unordered_set<pair<pair<int,int>,pair<int,int>>> bfs_path(unordered_set<pair<pair<int,int>,pair<int,int>>> maze,int n){
    vector<vector<bool>> visited(n,vector<bool>(n,false));
    vector<vector<pair<int,int>>> parent(n,vector<pair<int,int>>(n,{-1,-1}));
    queue<pair<int,int>> q;
    q.push({0,0});
    visited[0][0]=true;
    while(!q.empty()){
        pair<int,int> p = q.front();
        q.pop();
        int i = p.first,j = p.second;
        // cout<<i<<" "<<j<<endl;
        visited[i][j] = true;
        bfs_count++;
        if(p==make_pair(n-1,n-1)) break;
        if(i<n-1 && maze.find({{i+1,j},{i+1,j+1}})==maze.end() && !visited[i+1][j]){
            q.push({i+1,j});
            parent[i+1][j]={i,j};
        }
        if(i>0 && maze.find({{i,j},{i,j+1}})==maze.end() && !visited[i-1][j]){
            q.push({i-1,j});
            parent[i-1][j]={i,j};
        }
        if(j<n-1 && maze.find({{i,j+1},{i+1,j+1}})==maze.end() && !visited[i][j+1]){
            q.push({i,j+1});
            parent[i][j+1]={i,j};
        }
        if(j>0 && maze.find({{i,j},{i+1,j}})==maze.end() && !visited[i][j-1]){
            q.push({i,j-1});
            parent[i][j-1]={i,j};
        }
    }
    pair<int,int> end = {n-1,n-1};
    unordered_set<pair<pair<int,int>,pair<int,int>>> path;
    while(parent[end.first][end.second]!=make_pair(-1,-1)){
        pair<int,int> a = parent[end.first][end.second];
        if(a.first==end.first+1 || a.second==end.second+1){
            path.insert({end,a});
        }
        else{
            path.insert({a,end});
        }
        end = a;
    }
    return path;
}
void display_maze(unordered_set<pair<pair<int,int>,pair<int,int>>> maze,int n){
    for(int i=0;i<=n;i++){
        for(int j=0;j<n;j++){
            cout<<"* ";
            if(maze.find({{i,j},{i,j+1}})!=maze.end()){
                cout<<"* ";
            }
            else{
                cout<<"  ";
            }
        }
        cout<<"*"<<endl;
        for(int j=0;j<=n;j++){
            if(maze.find({{i,j},{i+1,j}})!=maze.end()){
                cout<<"*   ";
            }
            else{
                cout<<"    ";
            }
        }
        cout<<endl;
    }
}
//displaying the maze

