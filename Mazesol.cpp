#include <bits/stdc++.h>
#include <chrono>
using namespace std;
pair<int,int> find(pair<int,int> x,vector<vector<pair<int,int>>> &v){
    //union find using path compression
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
int dfs_count=0,bfs_count=0,a_count=0;
int dfs_duration=0,bfs_duration=0,a_duration=0,maze_creation_duration=0;
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
        // cout<<i<<" "<<j<<endl;
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