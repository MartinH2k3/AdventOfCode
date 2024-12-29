#include <algorithm>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <ranges>
#include <sstream>
#include <vector>
#include <string>
#include <set>
#include <ranges>
#include "helpers.h"
#include "dijsktra.h"

bool do_part2 = false;

size_t getPointIndex(const Point& p, const int width) {
    return p.y * width + p.x;
}

int main() {
    std::ifstream file("bob.txt");
    std::string line;
    std::vector<Point> obstacles;
    int scanned = 0;
    int limit = 1024;
    while (std::getline(file, line)) {
        if (scanned >= limit) {
            break;
        }
        std::ranges::replace(line.begin(), line.end(), ',', ' ');
        std::istringstream iss(line);
        int x, y;
        iss >> x >> y;
        obstacles.emplace_back(x, y);
        scanned++;
    }

    std::map<size_t, std::vector<std::pair<size_t, size_t>>> adjacencies;
    int size = 71;
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            Point current(x, y);
            std::vector<std::pair<size_t, size_t>> neighbors;
            if (std::ranges::find(obstacles, current) != obstacles.end()) {
                continue;
            }
            for (auto temp_neighbors = current.neighbors(size, size);
                const auto& neighbor : temp_neighbors) {
                if (std::ranges::find(obstacles, neighbor) == obstacles.end()) {
                    neighbors.emplace_back(1, getPointIndex(neighbor, size));
                }
                }
            adjacencies[getPointIndex(current, size)] = neighbors;
        }
    }
    auto dijkstra = DijkstraSolver(adjacencies);
    std::cout << dijkstra.shortest_path_cost(0, size*size-1) << std::endl;

    // Part 2 I did manual binary search changing the limit variable
    return 0;
}