#include <algorithm>
#include <iostream>
#include <fstream>
#include <ranges>
#include <string>
#include <map>
#include <set>


bool do_part2 = false;

std::pair<int, int> twice_thrice(const std::string& s) {
    std::map<char, int> letter_counts;
    for (char c : s) {
        letter_counts[c]++;
    }
    int twice = 0;
    int thrice = 0;
    for (const auto& [letter, count] : letter_counts) {
        if (count == 2) {
            twice = 1;
        } else if (count == 3) {
            thrice = 1;
        }
    }
    return {twice, thrice};
}

int main() {
    std::ifstream file("bob.txt");
    std::string line;
    int twice_count = 0, thrice_count = 0;
    std::set<std::string> seen;
    bool part2done = false;
    while (std::getline(file, line)) {
        if (!part2done) {
            for (auto left_out_char = 0; left_out_char < line.size(); left_out_char++) {
                if (std::string substring = line.substr(0, left_out_char) + ' ' +line.substr(left_out_char + 1);
                    seen.contains(substring)) {
                    std::cout << "Part 2: " << substring << std::endl;
                    part2done = true;
                    }
                else {
                    seen.insert(substring);
                }
            }
        }
        auto [twice, thrice] = twice_thrice(line);
        twice_count += twice;
        thrice_count += thrice;
    }
    std::cout << "Part 1: " << twice_count * thrice_count << std::endl;
    return 0;
}