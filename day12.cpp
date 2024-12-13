#include <algorithm>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <set>
#include <ranges>
#include <list>
#include <map>

bool do_part2 = false;

struct DisjointSet {
    std::vector <int> parent;
    std::vector <int> rank;
    int height;
    int width;
    explicit DisjointSet(const int height, const int width) : height(height), width(width) {
        parent.resize(height * width);
        rank.resize(height * width);
        for (int i = 0; i < height * width; i++) {
            parent[i] = i;
            rank[i] = 0;
        }
    }
    int find(const int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    void unite(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) {
            return;
        }
        if (rank[x] < rank[y]) {
            parent[x] = y;
        } else {
            parent[y] = x;
            if (rank[x] == rank[y]) {
                rank[x]++;
            }
        }
    }
    int fence_cost() {
        //directions
        std::vector dx = {-1, 0, 1, 0};
        std::vector dy = {0, -1, 0, 1};
        // map of perimeters and areas of cells
        std::map<int, int> areas;
        std::map<int, int> perimeters;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                auto root = find(y * width + x);
                areas[root] += 1;

                int add = 0;
                for (int i = 0; i < 4; i++) {
                    const int new_y1 = y + dy[i];
                    const int new_x1 = x + dx[i];
                    const bool new1_out = new_y1 < 0 || new_y1 >= height || new_x1 < 0 || new_x1 >= width || find(new_y1 * width + new_x1) != root;
                    if (!do_part2) {
                        add += new1_out;
                        continue;
                    }
                    const int new_y2 = y + dy[(i + 1) % 4];
                    const int new_x2 = x + dx[(i + 1) % 4];
                    const bool new2_out = new_y2 < 0 || new_y2 >= height || new_x2 < 0 || new_x2 >= width || find(new_y2 * width + new_x2) != root;
                    const int diag_y = y + dy[i] + dy[(i + 1) % 4];
                    const int diag_x = x + dx[i] + dx[(i + 1) % 4];
                    const bool diag_out = diag_y < 0 || diag_y >= height || diag_x < 0 || diag_x >= width || find(diag_y * width + diag_x) != root;
                    if (new1_out && new2_out) {
                        add += 1;
                    }
                    if (!new1_out && !new2_out && diag_out) {
                        add += 1;
                    }

                }
                perimeters[root] += add;
            }
        }
        std::map<int, int> costs;
        int cost = 0;
        for (const auto& key : std::views::keys(areas)) {
            cost += areas[key] * perimeters[key];
            costs[key] = areas[key] * perimeters[key];
        }
        return cost;
    }
};


int main() {
    std::ifstream file("bob.txt");
    std::string line;
    std::vector<std::string> garden_plot;
    while (std::getline(file, line)) {
        garden_plot.push_back(line);
    }

    const int height = garden_plot.size();
    const int width = garden_plot[0].size();
    DisjointSet ds(height, width);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i > 0 && garden_plot[i - 1][j] == garden_plot[i][j]) {
                ds.unite(i * width + j, (i - 1) * width + j);
            }
            if (j > 0 && garden_plot[i][j - 1] == garden_plot[i][j]) {
                ds.unite(i * width + j, i * width + j - 1);
            }
        }
    }

    std::cout<< "Part 1: " << ds.fence_cost()<<std::endl;
    do_part2 = true;
    std::cout<< "Part 1: " << ds.fence_cost()<<std::endl;


    return 0;
}
