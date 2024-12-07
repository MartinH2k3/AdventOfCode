#include <algorithm>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

bool do_part2 = false;

int64_t concat(int64_t a, int64_t b) {
    int64_t c = b;
    while (c) {
        a *= 10;
        c /= 10;
    }
    return a + b;
}

int dfs(const vector<int>& numbers, int index, int64_t curr, int64_t expected) {
    if (curr > expected) {
        return 0;
    }
    if (index == numbers.size()) {
        return curr == expected;
    }
    int output = 0;
    output += dfs(numbers, index+1, curr+numbers[index], expected);
    //output += dfs(numbers, index+1, curr-numbers[index], expected);
    output += dfs(numbers, index+1, curr*numbers[index], expected);
    //output += dfs(numbers, index+1, curr/numbers[index], expected);
    if (do_part2) {
        output += dfs(numbers, index+1, concat(curr, numbers[index]), expected);
    }
    return output;
}

int main() {
    // scanning the file
    std::ifstream file("bob.txt");
    if (!file.is_open()) {
        return 200;
    }
    vector<int64_t> results;
    vector<vector<int>> numbers;
    string line;
    while (getline(file, line)) {
        std::ranges::replace(line.begin(), line.end(), ':', ' ');
        istringstream iss(line);

        int64_t result;
        iss >> result;
        results.push_back(result);

        vector<int> temp;
        int number;
        while (iss >> number) {
            temp.push_back(number);
        }
        numbers.push_back(temp);
    }

    for (int i = 0; i<2; i++) {
        int64_t output = 0;
        do_part2 = i;
        for (int i = 0; i<numbers.size(); i++) {
            if(dfs(numbers[i], 1, numbers[i][0], results[i])) {
                output += results[i];
            }
        }
        cout << "Part " << i+1 << ": " << output << endl;
    }
    return 0;
}
