#include <algorithm>
#include <any>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <ranges>
#include <vector>
#include <string>
#include <stack>
#include <regex>

enum struct tile {
    empty,
    wall,
    seen,
};

enum struct direction {
    up,
    down,
    left,
    right
};

std::vector<std::vector<tile>> plan;
std::map<std::pair<int, int>, std::set<direction>> visited;

struct guard {
    int x;
    int y;
    direction dir;
    bool within_bounds(const size_t height=plan.size(), const size_t width=plan[0].size()) const {
        return y >= 0 && y < height && x >= 0 && x < width;
    }

    void rotate90deg() {
        switch (dir) {
            case direction::up:
                dir = direction::right;
            break;
            case direction::down:
                dir = direction::left;
            break;
            case direction::left:
                dir = direction::up;
            break;
            case direction::right:
                dir = direction::down;
            break;
        }
    }

    bool viable_move() const {
        return plan[y][x] != tile::wall;
    }

    void revert_move() {
        switch (dir) {
            case direction::up:
                y++;
            break;
            case direction::down:
                y--;
            break;
            case direction::left:
                x++;
            break;
            case direction::right:
                x--;
            break;
        }
    }

    void move() {
        switch (dir) {
            case direction::up:
                y--;
                break;
            case direction::down:
                y++;
                break;
            case direction::left:
                x--;
                break;
            case direction::right:
                x++;
                break;
        }
        if (!within_bounds()) {
            return;
        }
        if (!viable_move()) {
            revert_move();
            rotate90deg();
            move();
        }
    }
};

int main() {
    // scanning the file
    std::ifstream file("bob.txt");
    if (!file.is_open()) {
        return 200;
    }
    std::string line;
    guard gertrude{};
    int row_number = 0;
    while(std::getline(file, line)) {
        std::vector<tile> row;
        for (int i = 0; i < line.size(); i++) {
            char c = line[i];
            switch (c) {
                case '#':
                    row.push_back(tile::wall);
                    break;
                case '.':
                    row.push_back(tile::empty);
                    break;
                case '^':
                    row.push_back(tile::seen);
                    gertrude.x = i;
                    gertrude.y = row_number;
                    gertrude.dir = direction::up;
                    break;
                case '>':
                    row.push_back(tile::seen);
                    gertrude.x = i;
                    gertrude.y = row_number;
                    gertrude.dir = direction::right;
                    break;
                case 'v':
                    row.push_back(tile::seen);
                    gertrude.x = i;
                    gertrude.y = row_number;
                    gertrude.dir = direction::down;
                    break;
                case '<':
                    row.push_back(tile::seen);
                    gertrude.x = i;
                    gertrude.y = row_number;
                    gertrude.dir = direction::left;
                    break;
                default:
                    throw std::runtime_error("Invalid character in input");
            }
        }
        plan.push_back(row);
        row_number++;
    }
    auto plan_copy = plan;
    auto gert_copy = gertrude;
    //for (int i = 0; i < 8; i++){
    while (gertrude.within_bounds()) {
        plan[gertrude.y][gertrude.x] = tile::seen;
        gertrude.move();
    }
    // count seen tiles
    int seen_tiles = 0;
    for (auto row : plan) {
        for (auto t : row) {
            if (t == tile::seen) {
                seen_tiles++;
            }
        }
    }
    std::cout << "Seen tiles: " << seen_tiles << std::endl;

    plan = plan_copy;
    size_t loops = 0;
    for (size_t i = 0; i < plan.size(); i++) {
        for (size_t j = 0; j < plan[0].size(); j++) {
            if (plan[i][j] == tile::wall) {
                continue;
            }
            gertrude = gert_copy;
            plan[i][j] = tile::wall;
            std::vector<std::vector<std::set<direction>>> visited_directions(plan.size(), std::vector<std::set<direction>>(plan[0].size(), std::set<direction>()));
            while (gertrude.within_bounds()) {
                if (visited_directions[gertrude.y][gertrude.x].contains(gertrude.dir)) {
                    loops++;
                    break;
                }
                visited_directions[gertrude.y][gertrude.x].insert(gertrude.dir);
                gertrude.move();
            }
            plan[i][j] = tile::empty;
        }
    }
    std::cout << "Loops: " << loops << std::endl;
    return 0;
}
