#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

bool fits(const std::vector<int>& key, const std::vector<int>& lock) {
    for (size_t i = 0; i < key.size(); i++) {
        if (key[i] + lock[i] > 7) {
            return false;
        }
    }
    return true;
}

int main() {
    std::system("python refactor_input.py");
    std::ifstream file("input.txt");

    std::vector<std::vector<int>> locks;
    std::vector<std::vector<int>> keys;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<int> row;
        char type;
        iss >> type;

        int temp;
        while (iss >> temp)
            row.push_back(temp);

        if (type == 'K')
            keys.push_back(row);
        else
            locks.push_back(row);
    }

    int output = 0;
    for (const auto& key : keys) {
        for (const auto& lock : locks) {
            if (fits(key, lock)) {
                output++;
            }
        }
    }
    std::cout << output << std::endl;
    return 0;
}


