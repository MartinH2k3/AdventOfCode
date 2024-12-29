#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <chrono>
#include "helpers.h"

int max_depth = 26;

struct mem_key {
    int start;
    int end;
    int depth;
    bool operator==(const mem_key& other) const {
        return start == other.start && end == other.end && depth == other.depth;
    }
    auto operator<=>(const mem_key& other) const {
        if (const auto cmp = start <=> other.start; cmp != 0) {
            return cmp;
        }
        if (const auto cmp = end <=> other.end; cmp != 0) {
            return cmp;
        }
        return depth <=> other.depth;
    }
};

std::map<mem_key, size_t> mem;

Point getKeyCoordinates(int key) {
    switch (key) {
        case 7: return {0, 0};
        case 8: return {1, 0};
        case 9: return {2, 0};
        case 4: return {0, 1};
        case 5: return {1, 1};
        case 6: return {2, 1};
        case 1: return {0, 2};
        case 2: return {1, 2};
        case 3: return {2, 2};
        case 0: return {1, 3};
        case 10: return {2, 3};
        default: throw std::invalid_argument("Invalid key");
    }
}

Point getKeyCoordinates(char key) {
    switch (key) {
        case 'A': return {2, 0};
        case '^': return {1, 0};
        case '<': return {0, 1};
        case 'V': return {1, 1};
        case '>': return {2, 1};
        default: throw std::invalid_argument("Invalid key");
    }
}

bool can_start_vertical(Point start, Point end, bool init=false) {
    if (init){
        return start.x != 0 || end.y != 3;
    }
    return start.x != 0 || end.y != 0;
}

size_t move_count(char start, char end, int depth) {
    if (mem.contains({start, end, depth}))
        return mem[{start, end, depth}];
    auto end_point = getKeyCoordinates(end);
    auto start_point = getKeyCoordinates(start);
    if (depth == max_depth)
        return 0;
    auto diff = end_point - start_point;
    char vertical_move = diff.y > 0 ? 'V' : '^';
    char horizontal_move = diff.x > 0 ? '>' : '<';

    size_t click_count = abs(diff.x) + abs(diff.y); // clicking horizontal arrow (<>), vertical arrow (^V)
    if (click_count == 0) // no movement needed
        return 0;
    if (diff.y == 0)
        return move_count('A', horizontal_move, depth+1) + move_count(horizontal_move, 'A', depth+1) + click_count;
    if (diff.x == 0)
        return move_count('A', vertical_move, depth+1) + move_count(vertical_move, 'A', depth+1) + click_count;

    size_t horizontal_first = SIZE_T_MAX, vertical_first = SIZE_T_MAX;
    if (can_start_vertical(start_point, end_point))
        vertical_first = move_count('A', vertical_move, depth+1) + move_count(vertical_move, horizontal_move, depth+1) + move_count(horizontal_move, 'A', depth+1);
    if (can_start_vertical(end_point, start_point))
        horizontal_first = move_count('A', horizontal_move, depth+1) + move_count(horizontal_move, vertical_move, depth+1) + move_count(vertical_move, 'A', depth+1);

    mem[{start, end, depth}] = std::min(horizontal_first, vertical_first) + click_count;
    return mem[{start, end, depth}];
}

size_t move_count(int start, int end) {
    auto endpoint = getKeyCoordinates(end);
    auto startpoint = getKeyCoordinates(start);
    auto diff = endpoint - startpoint;
    char vertical_move = diff.y > 0 ? 'V' : '^';
    char horizontal_move = diff.x > 0 ? '>' : '<';

    size_t click_count = abs(diff.x) + abs(diff.y) + 1;
    if (click_count == 1) // no movement needed, just submit
        return 1;
    if (diff.y == 0)
        return move_count('A', horizontal_move, 1) + move_count(horizontal_move, 'A', 1) + click_count;
    if (diff.x == 0)
        return move_count('A', vertical_move, 1) + move_count(vertical_move, 'A', 1) + click_count;
    size_t horizontal_first = SIZE_T_MAX, vertical_first = SIZE_T_MAX;
    if (can_start_vertical(startpoint, endpoint, true))
        vertical_first = move_count('A', vertical_move, 1) + move_count(vertical_move, horizontal_move, 1) + move_count(horizontal_move, 'A', 1);
    if (can_start_vertical(endpoint, startpoint, true))
        horizontal_first = move_count('A', horizontal_move, 1) + move_count(horizontal_move, vertical_move, 1) + move_count(vertical_move, 'A', 1);
    //std::cout << click_count << std::endl;
    return std::min(horizontal_first, vertical_first) + click_count;
}

// dokopy distance 2, 2 ale tahov 4, 4

int main(){
    std::ifstream file("input.txt");
    std::string line;
    size_t output = 0;
    while (std::getline(file, line)) {
        int start = 10;
        int next;
        size_t row_output = 0;
        for (char c: line) {
            if (c == 'A') {
                row_output += move_count(start, 10);
                break;
            }
            next = c - '0';
            row_output += move_count(start, next);
            start = next;
        }
        std::stringstream ss(line);
        int multiplier;
        ss >> multiplier;
        output += row_output * multiplier;
    }
    std::cout << "Output: " << output << " (for part 1, change max_depth to 3 at the top of the code)"<< std::endl;

    return 0;
}