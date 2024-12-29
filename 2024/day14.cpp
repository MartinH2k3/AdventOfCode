#include <algorithm>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <set>
#include <ranges>
#include <numeric>
#include <algorithm>
#include <list>
#include <map>
#include <cstring>

bool do_part2 = false;

struct point {
    int x, y, vx, vy;
};

int main() {
    std::ifstream file("bob.txt");
    std::string line;
    int q1 = 0, q2 = 0, q3 = 0, q4 = 0,
    width = 101, height = 103, w_split = width/2, h_split = height/2,
    rounds = 100;
    // for part 2
    std::vector<point> points;
    while (std::getline(file, line)) {
        // makes it so I can use string stream to scan integers
        std::ranges::replace(line, ',', ' ');
        std::ranges::replace(line, 'p', ' ');
        std::ranges::replace(line, 'v', ' ');
        std::ranges::replace(line, '=', ' ');
        std::istringstream buffer(line);
        int px, py, vx, vy;
        buffer >> px >> py >> vx >> vy;
        points.push_back({px, py, vx, vy});
        px = (px + vx * rounds)%width;
        if (px < 0) {
            px += width;
        }
        py = (py + vy * rounds)%height;
        if (py < 0) {
            py += height;
        }
        if (px < w_split && py < h_split) {
            q1++;
        } else if (px > w_split && py < h_split) {
            q2++;
        } else if (px < w_split && py > h_split) {
            q3++;
        } else if (px > w_split && py > h_split) {
            q4++;
        }
    }
    std::cout << "Part 1: " << q1*q2*q3*q4 << std::endl;

    std::ofstream out("rob.txt");
    int grid[width][height];
    for (int i = 0; i < 20000; i++) {
        std::memset(grid, 0, sizeof(grid));
        for (auto& p : points) {
            grid[p.x][p.y] = 1;
            p.x += p.vx;
            p.x = (p.x + width)%width;
            p.y += p.vy;
            p.y = (p.y + height)%height;
        }

        out << i << "\n";
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                switch (grid[x][y]) {
                    case 1:
                        std::cout << '#';
                        break;
                    case 0:
                        std::cout << '.';
                        break;
                }
            }
            out << "\n";
        }
    }
    return 0;
}
