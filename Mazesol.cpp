#include <bits/stdc++.h>
#include <chrono>
using namespace std;

pair<int,int> find(pair<int,int> x,vector<vector<pair<int,int>>> &v){
    pair<int,int> p = x;
    while(v[p.first][p.second]!=p){
        p = v[p.first][p.second];
    }
    while(x!=p){
        pair<int,int> parent = v[x.first][x.second];
        v[x.first][x.second] = p;
        x = parent;
    }
    return x;
}

void Union(pair<int,int> a,pair<int,int> b,vector<vector<pair<int,int>>> &v){
    pair<int,int> p = find(a,v);
    v[p.first][p.second]=find(b,v);
}

namespace std {
    template <>
    struct hash<std::pair<std::pair<int, int>, std::pair<int, int>>> {
        size_t operator()(const std::pair<std::pair<int, int>, std::pair<int, int>>& p) const {
            size_t hash1 = std::hash<int>()(p.first.first);
            size_t hash2 = std::hash<int>()(p.first.second);
            size_t hash3 = std::hash<int>()(p.second.first);
            size_t hash4 = std::hash<int>()(p.second.second);
            return hash1 ^ (hash2 << 1) ^ (hash3 << 2) ^ (hash4 << 3);
        }
    };
}

int bfs_count=0,dfs_count=0,dijk_count=0,a_count=0;
int bfs_duration=0,dfs_duration=0,dijk_duration=0,a_duration=0,maze_creation_duration=0;

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

unordered_set<pair<pair<int,int>,pair<int,int>>> dfs_path(unordered_set<pair<pair<int,int>,pair<int,int>>> maze,int n){
    vector<vector<bool>> visited(n,vector<bool>(n,false));
    vector<vector<pair<int,int>>> parent(n,vector<pair<int,int>>(n,{-1,-1}));
    vector<pair<int,int>> st;
    st.push_back({0,0});
    visited[0][0]=true;
    while(!st.empty()){
        pair<int,int> p = st.back();
        st.pop_back();
        int i = p.first,j = p.second;
        visited[i][j]=true;
        dfs_count++;
        if(p==make_pair(n-1,n-1)) break;
        if(i<n-1 && maze.find({{i+1,j},{i+1,j+1}})==maze.end() && !visited[i+1][j]){
            st.push_back({i+1,j});
            parent[i+1][j]={i,j};
        }
        if(i>0 && maze.find({{i,j},{i,j+1}})==maze.end() && !visited[i-1][j]){
            st.push_back({i-1,j});
            parent[i-1][j]={i,j};
        }
        if(j<n-1 && maze.find({{i,j+1},{i+1,j+1}})==maze.end() && !visited[i][j+1]){
            st.push_back({i,j+1});
            parent[i][j+1]={i,j};
        }
        if(j>0 && maze.find({{i,j},{i+1,j}})==maze.end() && !visited[i][j-1]){
            st.push_back({i,j-1});
            parent[i][j-1]={i,j};
        }
    }
    pair<int,int> end = {n-1,n-1};
    unordered_set<pair<pair<int,int>,pair<int,int>>> path;
    while(parent[end.first][end.second]!=make_pair(-1,-1)){
        pair<int,int> a = parent[end.first][end.second];
        if(end.first==a.first+1||end.second==a.second+1){
            path.insert({a,end});
        }
        else{
            path.insert({end,a});
        }
        end = a;
    }
    return path;
}

unordered_set<pair<pair<int,int>,pair<int,int>>> dijk_path(unordered_set<pair<pair<int,int>,pair<int,int>>> &maze, int n){
    vector<vector<int>> dist(n, vector<int>(n, INT_MAX));
    vector<vector<bool>> visited(n,vector<bool>(n,false));
    vector<vector<pair<int, int>>> parent(n, vector<pair<int, int>>(n, {-1, -1}));
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<>> pq;
    dist[0][0] = 0;
    pq.push({0,{0,0}});
    visited[0][0]=true;
    while (!pq.empty()) {
        auto [d, p] = pq.top();
        pq.pop();
        int i = p.first, j = p.second;
        visited[i][j]=true;
        if (p == make_pair(n-1,n-1)) break;
        dijk_count++;
        if(i<n-1 && maze.find({{i+1,j},{i+1,j+1}})==maze.end() && !visited[i+1][j]){
            int newDist = d+1; 
            if (newDist < dist[i+1][j]) {
                dist[i+1][j] = newDist;
                parent[i+1][j] = {i,j};
                pq.push({newDist, {i+1,j}});
            }
        }
        if(i>0 && maze.find({{i,j},{i,j+1}})==maze.end() && !visited[i-1][j]){
            int newDist = d+1; 
            if (newDist < dist[i-1][j]) {
                dist[i-1][j] = newDist;
                parent[i-1][j] = {i,j};
                pq.push({newDist, {i-1,j}});
            }
        }
        if(j<n-1 && maze.find({{i,j+1},{i+1,j+1}})==maze.end() && !visited[i][j+1]){
            int newDist = d+1; 
            if (newDist < dist[i][j+1]) {
                dist[i][j+1] = newDist;
                parent[i][j+1] = {i,j};
                pq.push({newDist, {i,j+1}});
            }
        }
        if(j>0 && maze.find({{i,j},{i+1,j}})==maze.end() && !visited[i][j-1]){
            int newDist = d+1; 
            if (newDist < dist[i][j-1]) {
                dist[i][j-1] = newDist;
                parent[i][j-1] = {i,j};
                pq.push({newDist, {i,j-1}});
            }
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

struct change{
    int n;
    change(int n): n(n) {}
    bool operator()(pair<int,int> a,pair<int,int> b){
        int costa = 2*(n-1)-(a.first+a.second);
        int costb = 2*(n-1)-(b.first+b.second);
        if(costa<costb){
            return b<a;
        }
        else{
            return a<b;
        }
    }
};

unordered_set<pair<pair<int,int>,pair<int,int>>> a_path(unordered_set<pair<pair<int,int>,pair<int,int>>> maze,int n){
    vector<vector<bool>> visited(n,vector<bool>(n,false));
    vector<vector<pair<int,int>>> parent(n,vector<pair<int,int>>(n,{-1,-1}));
    change customComparator(n);
    priority_queue<pair<int,int>,vector<pair<int,int>>, change> st(customComparator);
    st.push({0,0});
    visited[0][0]=true;
    while(!st.empty()){
        pair<int,int> p = st.top();
        st.pop();
        int i = p.first,j = p.second;
        visited[i][j]=true;
        if(p==make_pair(n-1,n-1)) break;
        a_count++;
        if(i<n-1 && maze.find({{i+1,j},{i+1,j+1}})==maze.end() && !visited[i+1][j]){
            st.push({i+1,j});
            parent[i+1][j]={i,j};
        }
        if(i>0 && maze.find({{i,j},{i,j+1}})==maze.end() && !visited[i-1][j]){
            st.push({i-1,j});
            parent[i-1][j]={i,j};
        }
        if(j<n-1 && maze.find({{i,j+1},{i+1,j+1}})==maze.end() && !visited[i][j+1]){
            st.push({i,j+1});
            parent[i][j+1]={i,j};
        }
        if(j>0 && maze.find({{i,j},{i+1,j}})==maze.end() && !visited[i][j-1]){
            st.push({i,j-1});
            parent[i][j-1]={i,j};
        }
    }
    pair<int,int> end = {n-1,n-1};
    unordered_set<pair<pair<int,int>,pair<int,int>>> path;
    while(parent[end.first][end.second]!=make_pair(-1,-1)){
        pair<int,int> a = parent[end.first][end.second];
        if(end.first==a.first+1||end.second==a.second+1){
            path.insert({a,end});
        }
        else{
            path.insert({end,a});
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
void display(unordered_set<pair<pair<int,int>,pair<int,int>>> maze,unordered_set<pair<pair<int,int>,pair<int,int>>> s,int n){
    for(int i=0;i<=n;i++){
        for(int j=0;j<n;j++){
            cout<<"* ";
            if(maze.find({{i,j},{i,j+1}})!=maze.end()){
                cout<<"* ";
            }
            else if(s.find({{i-1,j},{i,j}})!=s.end()){
                cout<<"# ";
            }
            else{
                cout<<"  ";
            }
        }
        cout<<"*"<<endl;
        for(int j=0;j<=n;j++){
            if(maze.find({{i,j},{i+1,j}})!=maze.end()){
                cout<<"* ";
            }
            else if(s.find({{i,j-1},{i,j}})!=s.end()){
                cout<<"# ";
            }
            else{
                cout<<"  ";
            }
            if(i==n-1 && j==n-1){
                cout<<"# ";
            }
            else if(s.find({{i,j},{i,j+1}})!=s.end()){
                cout<<"# "; 
            }
            else if(s.find({{i,j},{i+1,j}})!=s.end()||s.find({{i-1,j},{i,j}})!=s.end()){
                cout<<"# ";
            }
            else{
                cout<<"  ";
            }
        }
        cout<<endl;
    }
}
void Sorting_algos(int bfs, int dfs, int dijk_duration, int a_duration) {
    vector<pair<int, string>> durations = {
        {dfs_duration,"Depth First Search"},
        {bfs_duration,"Breadth First Search"},
        {a_duration,"A* search"},
        {dijk_duration,"Dijkstra's"}
    };
    sort(durations.begin(), durations.end());
    for (size_t i = 0; i < durations.size(); ++i) {
        cout << durations[i].second;
        if (i < durations.size() - 1) cout << " < ";
    }
    cout << endl;
}
int main(){
    int n;
    cout<<"Enter the dimension of the maze(nxn): ";
    cin>>n;
    vector<vector<int>> v(n,vector<int>(n));
    vector<pair<pair<int,int>,pair<int,int>>> edges;
    set<pair<int,int>> visited;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(i<n-1 && visited.find({i,j})==visited.end()){
                edges.push_back({{i,j},{i+1,j}});
            }
            if(j<n-1 && visited.find({i,j})==visited.end()){
                edges.push_back({{i,j},{i,j+1}});
            }
            visited.insert({i,j});
        }
    }
    auto start = chrono::high_resolution_clock::now();
    random_device rd;
    mt19937 g(rd());
    shuffle(edges.begin(),edges.end(),g);

    vector<vector<pair<int,int>>> parent(n,vector<pair<int,int>>(n));
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
           parent[i][j] = {i,j};
        }
    }
    unordered_set<pair<pair<int,int>,pair<int,int>>> path;
    for(int i=0;i<edges.size();i++){
        pair<int,int> a=edges[i].first,b=edges[i].second;
        if(find(a,parent)!=find(b,parent)){
            Union(a,b,parent);
            path.insert({a,b});
        }
    }
    unordered_set<pair<pair<int,int>,pair<int,int>>> maze;
    for(int i=0;i<=n;i++){
        for(int j=0;j<=n;j++){
            if(i<n && (path.find({{i,j-1},{i,j}})==path.end()|| j==0 || j==n)){
                maze.insert({{i,j},{i+1,j}});
            }
            if(j<n && (path.find({{i-1,j},{i,j}})==path.end()|| i==0 || i==n)){
                maze.insert({{i,j},{i,j+1}});
            }
        }
    }
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop-start);
    maze_creation_duration = duration.count();
    
    cout<<"\n\nGenerated Maze: \n\n";
    display_maze(maze,n);

    cout<<"\n\n\nBFS path for the maze:\n\n";
    start = chrono::high_resolution_clock::now();
    unordered_set<pair<pair<int,int>,pair<int,int>>> bfs_ans = bfs_path(maze,n);
    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(stop-start);
    bfs_duration = duration.count();
    display(maze,bfs_ans,n);

    cout<<"\n\n\nDFS path for the maze:\n\n";
    start = chrono::high_resolution_clock::now();
    unordered_set<pair<pair<int,int>,pair<int,int>>> dfs_ans = dfs_path(maze,n);
    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(stop-start);
    dfs_duration = duration.count();
    display(maze,dfs_ans,n);
    
    cout<<"\n\n\nDijkstra path for the maze:\n\n";
    start = chrono::high_resolution_clock::now();
    unordered_set<pair<pair<int, int>, pair<int, int>>> dijk_ans= dijk_path(maze, n);
    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(stop-start);
    dijk_duration = duration.count();
    display(maze,dijk_ans,n);
    
    cout<<"\n\n\nA* path for the maze:\n\n";
    start = chrono::high_resolution_clock::now();
    unordered_set<pair<pair<int,int>,pair<int,int>>> a_ans = a_path(maze,n);
    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(stop-start);
    a_duration = duration.count();
    display(maze,a_ans,n);
    
    cout<<endl<<endl;
    cout<<"The steps taken for solving the maze using Breadth First Search algorithm: "<<bfs_count<<endl;
    cout<<"The steps taken for solving the maze using Depth First Search algorithm: "<<dfs_count<<endl;
    cout<<"The steps taken for solving the maze using Dijkstra's algorithm:  "<<dijk_count<<endl;
    cout<<"The steps taken for solving the maze using A* Search algorithm:  "<<a_count<<endl<<endl;
    
    cout<<"The order of the steps taken: ";
    Sorting_algos(bfs_count, dfs_count, dijk_count, a_count);
    cout<<endl;
    
    cout<<endl<<endl;
    cout<<"The time taken for creation of the Maze: "<<maze_creation_duration<<endl;
    cout<<"The time taken for solving the maze using Breadth First Search algorithm:: "<<bfs_duration<<endl;
    cout<<"The time taken for solving the maze using Depth First Search algorithm: "<<dfs_duration<<endl;
    cout<<"The time taken for solving the maze using  Dijkstra's algorithm:"<<dijk_duration<<endl;
    cout<<"The time taken for solving the maze using A* Search algorithm: "<<a_duration<<endl<<endl;
    
    cout<<"The order of time taken: ";
    Sorting_algos(bfs_duration, dfs_duration, dijk_duration, a_duration);
    cout<<endl;
    return 0;
}
