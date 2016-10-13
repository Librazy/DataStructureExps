#pragma once

#ifndef BFS_defined
// ReSharper disable CppUnusedIncludeDirective
#include <iostream>
#include <map>
#include <tuple>
#include <utility> 
#include <vector>
#include <sstream>

std::vector<std::tuple<int, int>> BFS(size_t x1, size_t y1, size_t x2, size_t y2, size_t w, size_t h, std::string m){
    std::vector<std::vector<int>> map;
    std::vector<std::vector<int>> vis;
    std::vector<std::tuple<int, int, int>> q;
    std::vector<std::tuple<int, int>> ans;
    int d[4][2] = {{1,0},{0,-1},{-1,0},{0,1}};
    std::stringstream ss(m);
    
    for(size_t i = 0;i!=h;++i){
        map[i] = std::vector<int>(w);
        vis[i] = std::vector<int>(w);
    }
    q.reserve(w*h);
    for(size_t i = 0; i!=h;++i){
        for(size_t j = 0; j!=w;++j){
            ss>>map[i][j];
        }
    }
    size_t front = 0;
    q.emplace_back(std::make_tuple(y1,x1,-1));
    while(front < q.size()){
        for(size_t u = 0; u != 4; ++u) {
            int nh = std::get<0>(q[front]) + d[u][0];
            int nw = std::get<1>(q[front]) + d[u][1];
            if((nh < 0) || (nh >= h) || (nw < 0) || (nw >= w) || (map[nh][nw] == 1) || (vis[nh][nw] == 1)) {
            	continue;
            } else {
                if(nh == y2 && nw == x2){
                    ans.emplace_back(std::make_tuple(y2,x2));
                    size_t cur = front;
                    while(cur != -1){
                        ans.emplace_back(std::make_tuple(std::get<0>(q[cur]),std::get<1>(q[cur])));
                        cur = std::get<2>(q[cur]);
                    }
                    return ans;
                }
                vis[nh][nw] = 1;
                q.emplace_back(std::make_tuple(nh,nw,front));
            }
      }
      ++front;
    }
    return ans;
}


#define BFS_defined

#endif