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

int BFS_distance=0,DFS_distance=0,Dijkstra_distance=0,Astar_distance=0;
int BFS_duration=0,DFS_duration=0,Dijkstra_duration=0,Astar_duration=0,maze_creation_duration=0;

unordered_set<pair<pair<int,int>,pair<int,int>>> BFS_algo(unordered_set<pair<pair<int,int>,pair<int,int>>> maze,int n){
    vector<vector<bool>> visited(n,vector<bool>(n,false));
    vector<vector<pair<int,int>>> parent(n,vector<pair<int,int>>(n,{-1,-1}));
    vector<vector<int>> dist(n, vector<int>(n, INT_MAX));
    queue<pair<int,int>> q;
    q.push({0,0});
    visited[0][0]=true;
    dist[0][0]=0;
    while(!q.empty()){
        pair<int,int> p = q.front();
        q.pop();
        int i = p.first,j = p.second;
        visited[i][j] = true;
        if(p==make_pair(n-1,n-1)) break;
        if(i<n-1 && maze.find({{i+1,j},{i+1,j+1}})==maze.end() && !visited[i+1][j]){
            q.push({i+1,j});
            parent[i+1][j]={i,j};
            dist[i+1][j]=dist[i][j]+1;
        }
        if(i>0 && maze.find({{i,j},{i,j+1}})==maze.end() && !visited[i-1][j]){
            q.push({i-1,j});
            parent[i-1][j]={i,j};
            dist[i-1][j]=dist[i][j]+1;
        }
        if(j<n-1 && maze.find({{i,j+1},{i+1,j+1}})==maze.end() && !visited[i][j+1]){
            q.push({i,j+1});
            parent[i][j+1]={i,j};
            dist[i][j+1]=dist[i][j]+1;
        }
        if(j>0 && maze.find({{i,j},{i+1,j}})==maze.end() && !visited[i][j-1]){
            q.push({i,j-1});
            parent[i][j-1]={i,j};
            dist[i][j-1]=dist[i][j]+1;
        }
    }
    BFS_distance = dist[n-1][n-1];
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

unordered_set<pair<pair<int,int>,pair<int,int>>> DFS_algo(unordered_set<pair<pair<int,int>,pair<int,int>>> maze,int n){
    vector<vector<bool>> visited(n,vector<bool>(n,false));
    vector<vector<pair<int,int>>> parent(n,vector<pair<int,int>>(n,{-1,-1}));
    vector<vector<int>> distance(n, vector<int>(n, INT_MAX));
    vector<pair<int,int>> st;
    st.push_back({0,0});
    visited[0][0]=true;
    distance[0][0]=0;
    while(!st.empty()){
        pair<int,int> p = st.back();
        st.pop_back();
        int i = p.first,j = p.second;
        visited[i][j]=true;
        if(p==make_pair(n-1,n-1)) break;
        if(i<n-1 && maze.find({{i+1,j},{i+1,j+1}})==maze.end() && !visited[i+1][j]){
            st.push_back({i+1,j});
            parent[i+1][j]={i,j};
            distance[i+1][j]=distance[i][j]+1;
        }
        if(i>0 && maze.find({{i,j},{i,j+1}})==maze.end() && !visited[i-1][j]){
            st.push_back({i-1,j});
            parent[i-1][j]={i,j};
            distance[i-1][j]=distance[i][j]+1;
        }
        if(j<n-1 && maze.find({{i,j+1},{i+1,j+1}})==maze.end() && !visited[i][j+1]){
            st.push_back({i,j+1});
            parent[i][j+1]={i,j};
            distance[i][j+1]=distance[i][j]+1;
        }
        if(j>0 && maze.find({{i,j},{i+1,j}})==maze.end() && !visited[i][j-1]){
            st.push_back({i,j-1});
            parent[i][j-1]={i,j};
            distance[i][j-1]=distance[i][j]+1;
        }
    }
    DFS_distance=distance[n-1][n-1];
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

struct change{
    int n;
    change(int n): n(n) {}
    bool operator()(pair<int,int> a,pair<int,int> b){
        int costa=2*(n-1)-(a.first+a.second);
        int costb=2*(n-1)-(b.first+b.second);
        if(costa<costb){
            return b<a;
        }
        else{
            return a<b;
        }
    }
};

unordered_set<pair<pair<int,int>,pair<int,int>>> Astar_algo(unordered_set<pair<pair<int,int>,pair<int,int>>> maze,int n){
    vector<vector<bool>> visited(n,vector<bool>(n,false));
    vector<vector<pair<int,int>>> parent(n,vector<pair<int,int>>(n,{-1,-1}));
    vector<vector<int>> distance(n, vector<int>(n, INT_MAX));
    change customComparator(n);
    priority_queue<pair<int,int>,vector<pair<int,int>>, change> pq(customComparator);
    pq.push({0,0});
    visited[0][0]=true;
    distance[0][0]=0;
    while(!pq.empty()){
        pair<int,int> p=pq.top();
        pq.pop();
        int i = p.first,j = p.second;
        visited[i][j]=true;
        if(p==make_pair(n-1,n-1)) break;
        Astar_distance++;
        if(i<n-1 && maze.find({{i+1,j},{i+1,j+1}})==maze.end() && !visited[i+1][j]){
            pq.push({i+1,j});
            parent[i+1][j]={i,j};
            distance[i+1][j]=distance[i][j]+1;
        }
        if(i>0 && maze.find({{i,j},{i,j+1}})==maze.end() && !visited[i-1][j]){
            pq.push({i-1,j});
            parent[i-1][j]={i,j};
            distance[i-1][j]=distance[i][j]+1;
        }
        if(j<n-1 && maze.find({{i,j+1},{i+1,j+1}})==maze.end() && !visited[i][j+1]){
            pq.push({i,j+1});
            parent[i][j+1]={i,j};
            distance[i][j+1]=distance[i][j]+1;
        }
        if(j>0 && maze.find({{i,j},{i+1,j}})==maze.end() && !visited[i][j-1]){
            pq.push({i,j-1});
            parent[i][j-1]={i,j};
            distance[i][j-1]=distance[i][j]+1;
        }
    }
    Astar_distance=distance[n-1][n-1];
    pair<int,int> end={n-1,n-1};
    unordered_set<pair<pair<int,int>,pair<int,int>>> path;
    while(parent[end.first][end.second]!=make_pair(-1,-1)){
        pair<int,int> a=parent[end.first][end.second];
        if(end.first==a.first+1||end.second==a.second+1){
            path.insert({a,end});
        }
        else{
            path.insert({end,a});
        }
        end=a;
    }
    return path;
}

unordered_map<pair<pair<int, int>, pair<int, int>>, int> generate_weights(int n) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> weight_dist(1,10);
    unordered_map<pair<pair<int, int>, pair<int, int>>, int> weights;
    for (int i=0;i<n;++i){
        for (int j=0;j<n;++j){
            if (i<n-1) {
                weights[{{i,j},{i+1,j}}]=weight_dist(gen);
            }
            if (j<n-1){
                weights[{{i,j},{i,j+1}}]=weight_dist(gen);
            }
        }
    }
    return weights;
}

unordered_set<pair<pair<int,int>,pair<int,int>>> Dijkstra_algo(unordered_set<pair<pair<int,int>,pair<int,int>>> &maze, int n){
    vector<vector<int>> dist(n, vector<int>(n, INT_MAX));
    auto weights=generate_weights(n);
    vector<vector<bool>> visited(n,vector<bool>(n,false));
    vector<vector<pair<int, int>>> parent(n, vector<pair<int, int>>(n, {-1, -1}));
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<>> pq;
    dist[0][0]=0;
    pq.push({0,{0,0}});
    visited[0][0]=true;
    while (!pq.empty()) {
        auto [d,p]=pq.top();
        pq.pop();
        int i=p.first,j=p.second;
        visited[i][j]=true;
        if (p==make_pair(n-1,n-1)) break;
        if(i<n-1 && maze.find({{i+1,j},{i+1,j+1}})==maze.end() && !visited[i+1][j]){
            int weight=weights[{{i,j},{i+1,j}}];
            int new_dist=d+weight; 
            if (new_dist<dist[i+1][j]) {
                dist[i+1][j]=new_dist;
                parent[i+1][j]={i,j};
                pq.push({new_dist,{i+1,j}});
            }
        }
        if(i>0 && maze.find({{i,j},{i,j+1}})==maze.end() && !visited[i-1][j]){
            int weight=weights[{{i,j},{i-1,j}}];
            int new_dist=d+weight; 
            if (new_dist<dist[i-1][j]) {
                dist[i-1][j]=new_dist;
                parent[i-1][j]={i,j};
                pq.push({new_dist,{i-1,j}});
            }
        }
        if(j<n-1 && maze.find({{i,j+1},{i+1,j+1}})==maze.end() && !visited[i][j+1]){
            int weight=weights[{{i,j},{i,j+1}}];
            int new_dist=d+weight; 
            if (new_dist<dist[i][j+1]) {
                dist[i][j+1]=new_dist;
                parent[i][j+1]={i,j};
                pq.push({new_dist,{i,j+1}});
            }
        }
        if(j>0 && maze.find({{i,j},{i+1,j}})==maze.end() && !visited[i][j-1]){
            int weight=weights[{{i, j},{i,j-1}}];
            int new_dist=d+weight; 
            if (new_dist<dist[i][j-1]) {
                dist[i][j-1]=new_dist;
                parent[i][j-1]={i,j};
                pq.push({new_dist,{i,j-1}});
            }
        }
    }
    Dijkstra_distance=dist[n-1][n-1];
    pair<int,int> end={n-1,n-1};
    unordered_set<pair<pair<int,int>,pair<int,int>>> path;
    while(parent[end.first][end.second]!=make_pair(-1,-1)){
        pair<int,int> a=parent[end.first][end.second];
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

void print_maze(unordered_set<pair<pair<int,int>,pair<int,int>>> maze,int n){
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
        cout<<"x"<<endl;
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
void print_path(unordered_set<pair<pair<int,int>,pair<int,int>>> maze,unordered_set<pair<pair<int,int>,pair<int,int>>> s,int n){
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
void Sorting_algos(int bfs, int dfs, int dijk, int star) {
    vector<pair<int, string>> algos = {
        {dfs,"Depth First Search"},
        {bfs,"Breadth First Search"},
        {star,"A* search"},
        {dijk,"Dijkstra's algorithm"}
    };
    sort(algos.begin(), algos.end());
    for (size_t i = 0; i < algos.size(); ++i) {
        cout << algos[i].second;
        if (i<algos.size()-1) cout << " < ";
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
    int extra_edges=4*n/5;
    shuffle(edges.begin(), edges.end(), g); 

    for (int i=0; i<edges.size() && extra_edges>0; i++) {
        pair<int, int> a=edges[i].first;
        pair<int, int> b=edges[i].second;
        if (path.find({a, b})==path.end() && path.find({b, a})==path.end()) {
            path.insert({a, b});
            extra_edges--;
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
    print_maze(maze,n);

    cout<<"\n\n\nBFS path for the maze:\n\n";
    start = chrono::high_resolution_clock::now();
    unordered_set<pair<pair<int,int>,pair<int,int>>> BFS_path=BFS_algo(maze,n);
    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(stop-start);
    BFS_duration = duration.count();
    print_path(maze,BFS_path,n);

    cout<<"\n\n\nDFS path for the maze:\n\n";
    start = chrono::high_resolution_clock::now();
    unordered_set<pair<pair<int,int>,pair<int,int>>> DFS_path=DFS_algo(maze,n);
    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(stop-start);
    DFS_duration = duration.count();
    print_path(maze,DFS_path,n);
    
    cout<<"\n\n\nA* path for the maze:\n\n";
    start = chrono::high_resolution_clock::now();
    unordered_set<pair<pair<int,int>,pair<int,int>>> Astar_path=Astar_algo(maze,n);
    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(stop-start);
    Astar_duration = duration.count();
    print_path(maze,Astar_path,n);
    
    cout<<"\n\n\nDijkstra path for the maze with weighted edges:\n\n";
    start = chrono::high_resolution_clock::now();
    unordered_set<pair<pair<int, int>, pair<int, int>>> Dijkstra_path=Dijkstra_algo(maze, n);
    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(stop-start);
    Dijkstra_duration = duration.count();
    print_path(maze,Dijkstra_path,n);
    
    cout<<endl<<endl;
    cout<<"The distance taken by Breadth First Search algorithm to solve the maze: "<<BFS_distance<<endl;
    cout<<"The distance taken by Depth First Search algorithm to solve the maze: "<<DFS_distance<<endl;
    cout<<"The distance taken by A* Search algorithm to solve the maze: "<<Astar_distance<<endl;
    cout<<"The distance taken by Dijkstra's algorithm to solve the maze: "<<Dijkstra_distance<<endl<<endl;
    
    cout<<"The order of the distance travelled: ";
    Sorting_algos(BFS_distance, DFS_distance, Dijkstra_distance, Astar_distance);
    cout<<endl;
    
    cout<<endl<<endl;
    cout<<"The time taken for creating the Maze: "<<maze_creation_duration<<endl;
    cout<<"The time taken for solving the maze using Breadth First Search algorithm:: "<<BFS_duration<<endl;
    cout<<"The time taken for solving the maze using Depth First Search algorithm: "<<DFS_duration<<endl;
    cout<<"The time taken for solving the maze using A* Search algorithm: "<<Astar_duration<<endl;
    cout<<"The time taken for solving the maze using  Dijkstra's algorithm: "<<Dijkstra_duration<<endl<<endl;
    
    cout<<"The order of time taken: ";
    Sorting_algos(BFS_duration,DFS_duration, Dijkstra_duration, Astar_duration);
    cout<<endl;
    return 0;
}
