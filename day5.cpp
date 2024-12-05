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

std::map<int, std::set<int>> rules;

bool follows_rules(std::vector<int> numbers) {
    for (int i = 0; i<numbers.size(); i++) {
        for (int j = i+1; j<numbers.size(); j++) {
            if (rules.contains(numbers[j]) && rules[numbers[j]].contains(numbers[i])) {
                return false;
            }
        }
    }
    return true;
}

bool order_by_rules(int a, int b) {
    if (rules.contains(a) && rules[a].contains(b)) {
        return false;
    }
    return true;
}

int main() {
    // scanning the file
    std::ifstream file("bob.txt");
    if (!file.is_open()) {
        return 200;
    }
    std::string line;
    std::string rule_regex = "[0-9]+\\|[0-9]+";
    while (std::getline(file, line) && std::regex_match(line, std::regex(rule_regex))) {
        std::ranges::replace(line.begin(), line.end(), '|', ' ');
        std::istringstream buffer(line);
        int a, b;
        buffer >> a >> b;
        if (rules.contains(a)) {
            rules[a].insert(b);
        }
        else {
            rules[a] = {b};
        }
    }
    int output = 0;
    int part2_output = 0;
    while (std::getline(file, line)) {
        std::ranges::replace(line.begin(), line.end(), ',', ' ');
        std::vector<int> numbers;
        std::istringstream buffer(line);
        int temp;
        while (buffer >> temp) {
            numbers.push_back(temp);
        }
        if (follows_rules(numbers)) {
            output += numbers[numbers.size()/2];
        }
        else {
            std::ranges::sort(numbers.begin(), numbers.end(), order_by_rules);
            part2_output += numbers[numbers.size()/2];
        }
    }
    std::cout << "Part 1: " << output << std::endl;
    std::cout << "Part 2: " <<  part2_output << std::endl;
    return 0;
}
