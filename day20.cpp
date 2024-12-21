#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include "helpers.h"

bool do_part2 = true;
int cutoff = 100;

void search(const Point& curr, std::map<int, int>& shortcuts, std::map<Point, int>& path){
    int limit = do_part2? 20 : 2;
    for (int i = 0; i <= limit; i++){
        for (int j = 0; j <= limit-i; j++){
            std::set<Point> next;
            next.insert(curr + Point(i, j));
            next.insert(curr + Point(i, -j));
            next.insert(curr + Point(-i, j));
            next.insert(curr + Point(-i, -j));
            for (auto next_point: next){
                if (path.contains(next_point) &&
                    path[next_point] > path[curr] + i + j){
                    shortcuts[path[next_point] - path[curr] - i - j]++;
                }
            }
        }
    }
}

int main(){
    std::ifstream file("input.txt");
    std::string line;
    std::vector<std::string> grid;
    while (getline(file, line)){
        grid.push_back(line);
    }
    // find S in the grid
    Point start;
    for (int i=0; i<grid.size(); i++){
        for (int j=0; j<grid[i].size(); j++){
            if (grid[i][j] == 'S'){
                start = Point(j, i);
                break;
            }
        }
    }
    int width = grid[0].size(), height = grid.size(), path_length = 0;
    std::map<Point, int> path;
    Point curr = start;
    while (grid[curr.y][curr.x] != 'E'){
        path[curr] = path_length;
        for (auto neighbor : curr.neighbors(width, height)){
            if (grid[neighbor.y][neighbor.x] != '#' && !path.contains(neighbor)){
                curr = neighbor;
                path_length++;
                break;
            }
        }
    }
    path[curr] = path_length;

    std::map<int, int> shortcuts;
    for (auto [point, distance] : path){
        if (grid[point.y][point.x] == 'E'){
            continue;
        }
        search(point, shortcuts, path);
    }
    int output = 0;
    for (auto [distance, count] : shortcuts){
        if (distance >= cutoff){
            output += count;
        }
    }
    std::cout << output << std::endl;
    return 0;
}