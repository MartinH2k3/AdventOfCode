#include <algorithm>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <ranges>

bool within_bounds(const int x, const int y, const size_t bound_x, const size_t bound_y) {
    return x>=0 && x<bound_x && y>=0 && y<bound_y;
}

std::vector<std::vector<uint8_t>> topography;
bool do_part2 = false;

uint32_t dfs(const uint8_t level, int x, int y, std::set<std::pair<int, int>>& visited) {
    if (!within_bounds(x, y, topography.size(), topography[0].size())) {
        return 0;
    }
    if (topography[x][y]!=level) {
        return 0;
    }
    if (visited.contains({x, y})) {
        return 0;
    }
    if (!do_part2) visited.insert({x, y});
    if (level==9) {
        return 1;
    }
    return  dfs(level+1, x+1, y, visited) +
            dfs(level+1, x-1, y, visited) +
            dfs(level+1, x, y+1, visited) +
            dfs(level+1, x, y-1, visited);
}

int main() {
    // scanning the file
    std::ifstream file("bob.txt");
    std::string line;
    while (std::getline(file, line)) {
        std::vector<uint8_t> row;
        for (const char c : line) {
            row.push_back(c-'0');
        }
        topography.push_back(row);
    }
    uint32_t result = 0;
    for (int i=0; i<topography.size(); i++) {
        for (int j=0; j<topography[i].size(); j++) {
            std::set<std::pair<int, int>> visited;
            result += dfs(0, i, j, visited);
        }
    }
    std::cout << "Part 1: " << result << std::endl;
    do_part2 = true;
    result = 0;
    for (int i=0; i<topography.size(); i++) {
        for (int j=0; j<topography[i].size(); j++) {
            std::set<std::pair<int, int>> visited;
            result += dfs(0, i, j, visited);
        }
    }
    std::cout << "Part 2: " << result << std::endl;
    return 0;
}
