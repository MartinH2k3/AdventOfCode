#include <algorithm>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <ranges>
#include <numeric>



struct point {
    int x, y;
    bool within_bounds(int width, int height) const {
        return x >= 0 && x < width && y >= 0 && y < height;
    }
    point& operator+=(const point& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    point& operator-=(const point& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
};
point operator+(point a, const point& b) {
    a += b;
    return a;
}
point operator-(point a, const point& b) {
    a -= b;
    return a;
}
std::strong_ordering operator <=>(const point& a, const point& b) {
    return a.x == b.x ? a.y <=> b.y : a.x <=> b.x;
}

std::map<char, std::vector<point>> antennas;
bool do_part2 = false;

size_t count_antinodes(int width, int height) {
    std::set<point> antinodes;
    for (const auto& points : antennas | std::views::values) {
        for (int i = 0; i<points.size(); i++) {
            for (int j = i+1; j<points.size(); j++) {
                point distance = points[j] - points[i];
                if (do_part2) {
                    int gcd = std::gcd(distance.x, distance.y);
                    distance.x /= gcd;
                    distance.y /= gcd;
                    point antinode1 = points[i];
                    while (antinode1.within_bounds(width, height)) {
                        antinodes.insert(antinode1);
                        antinode1 += distance;
                    }
                    antinode1 = points[j] - distance;
                    while (antinode1.within_bounds(width, height)) {
                        antinodes.insert(antinode1);
                        antinode1 -= distance;
                    }
                    continue;
                }
                point antinode1 = points[i] - distance;
                point antinode2 = points[j] + distance;
                if (antinode1.within_bounds(width, height)) {
                    antinodes.insert(antinode1);
                }
                if (antinode2.within_bounds(width, height)) {
                    antinodes.insert(antinode2);
                }
            }
        }
    }
    return antinodes.size();
}
int main() {
    // scanning the file
    std::ifstream file("bob.txt");
    if (!file.is_open()) {
        return 200;
    }
    std::string line;
    int y{0}, x{0};
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        char c;
        x = 0;
        while (iss >> c) {
            if (c != '.') {
                antennas[c].push_back({x, y});
            }
            x++;
        }
        y++;
    }

    std::cout << "Part 1: " << count_antinodes(x, y) << std::endl;
    do_part2 = true;
    std::cout << "Part 2: " << count_antinodes(x, y) << std::endl;
    return 0;
}
