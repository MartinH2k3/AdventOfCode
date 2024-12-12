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

size_t pow(size_t base, size_t exp) {
    size_t output = 1;
    for (size_t i = 0; i < exp; i++) {
        output *= base;
    }
    return output;
}

size_t count_digits(size_t n) {
    size_t output = 0;
    while (n > 0) {
        n /= 10;
        output++;
    }
    return output;
}

void blink(std::map<size_t, size_t>& stones) {
    std::map<size_t, size_t> output;
    for (const auto item: stones) {
        if (item.first == 0) {
            output[1] += item.second;
        }
        else if (const auto digit_count = count_digits(item.first); digit_count%2 == 0) {
            auto half1 = item.first / pow(10, digit_count/2);
            auto half2 = item.first % pow(10, digit_count/2);
            output[half1] += item.second;
            output[half2] += item.second;
        }
        else {
            output[item.first * 2024] += item.second;
        }
    }
    stones = output;
}

int main() {
    std::ifstream file("bob.txt");
    std::string line;
    std::getline(file, line);
    std::istringstream buffer(line);
    std::map<size_t, size_t> stones;
    size_t stone;
    while (buffer >> stone) {
        stones[stone]++;
    }
    for (int i = 0; i < 75; i++) {
        blink(stones);
    }

    size_t output = 0;
    for (const auto item: std::views::values(stones)) {
        output += item;
    }

    std::cout << output << std::endl;
    return 0;
}
