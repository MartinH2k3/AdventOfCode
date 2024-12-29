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

bool do_part2 = false;

struct point {
    int64_t x, y;
    bool operator==(const point &other) const {
        return x == other.x && y == other.y;
    }
    bool operator!=(const point &other) const {
        return x != other.x || y != other.y;
    }
};

struct claw_machine {
    point a, b, prize;
};

point get_from_line(std::string line, const bool is_prize = false) {
    int64_t x, y;
    if (is_prize) {
        std::ranges::replace(line.begin(), line.end(), '=', ' ');
    } else {
        std::ranges::replace(line.begin(), line.end(), '+', ' ');
    }
    std::istringstream iss(line);
    std::string temp;
    if (is_prize) {
        iss >> temp >> temp >> x >> temp >> temp >> y;
    } else {
        iss >> temp >> temp >> temp >> x >> temp >> temp >> y;
    }
    return {x, y};
}


int64_t machine_cost(const claw_machine& machine) {
    // same direction or one of them is 0,0
    if (machine.b.x*machine.a.y - machine.b.y*machine.a.x == 0) {
        // use only b
        if (machine.prize.x % machine.b.x == 0 && machine.prize.y % machine.b.y == 0) {
            return machine.prize.x/machine.b.x;
        }
        // use only a
        if (machine.prize.x % machine.a.x == 0 && machine.prize.y % machine.a.y == 0) {
            return machine.prize.x/machine.a.x*3;
        }
        // we are not checking combinations, as I can't do it in acceptable time
        return -1;
    }
    int64_t a_count = (machine.b.x*machine.prize.y - machine.b.y*machine.prize.x)/machine.b.x*machine.a.y - machine.b.y*machine.a.x;
    int64_t b_count;
    if (machine.b.x) {
        b_count = (machine.prize.x - a_count*machine.a.x)/machine.b.x;
    } else {
        b_count = (machine.prize.y - a_count*machine.a.y)/machine.b.y;
    }
    if (a_count*machine.a.x + b_count*machine.b.x != machine.prize.x ||
        a_count*machine.a.y + b_count*machine.b.y != machine.prize.y) {
        return -1;
    }
    return a_count*3 + b_count;
}

int main() {
    std::ifstream file("bob.txt");
    std::string line;
    std::vector<claw_machine> claw_machines;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        const point a = get_from_line(line);
        std::getline(file, line);
        const point b = get_from_line(line);
        std::getline(file, line);
        const point prize = get_from_line(line, true);
        claw_machines.push_back({a, b, prize});
    }

    int64_t output = 0;
    for (const auto& machine : claw_machines) {
        if (const auto machine_output = machine_cost(machine);
            machine_output != -1) {
            output += machine_output;
        }
    }
    std::cout << "Part 1: " << output << std::endl;
    for (auto& machine : claw_machines) {
        machine.prize.x += 10000000000000;
        machine.prize.y += 10000000000000;
    }
    output = 0;
    for (const auto& machine : claw_machines) {
        if (const auto machine_output = machine_cost(machine);
            machine_output != -1) {
            output += machine_output;
        }
    }
    std::cout << "Part 2: " << output << std::endl;
    return 0;
}
