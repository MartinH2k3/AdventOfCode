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

size_t calc_checksum(const std::vector<int>& disk) {
    size_t checksum = 0;
    for (size_t i = 0; i < disk.size(); i++) {
        if (disk[i] == -1) {
            continue;
        }
        checksum += i*disk[i];
    }
    return checksum;
}

void fill_in(std::vector<int>& disk, int id, int count, int pos) {
    int spaces = 0;
    for (int i = 0; i < pos; i++) {
        if (disk[i] == -1) {
            spaces++;
            if (spaces == count) {
                for (int j = 0; j < count; j++) {
                    disk[i-j] = id;
                    disk[pos+j] = -1;
                }
                break;
            }
        } else {
            spaces = 0;
        }
    }
}

int main() {
    // scanning the file
    std::ifstream file("bob.txt");
    std::string content;
    file >> content;
    int index = 0;
    std::vector<int> disk;
    for (size_t i = 0; i < content.size(); i+=2) {
        int temp = content[i] - '0';
        int index = i/2;
        for (size_t j = 0; j < temp; j++) {
            disk.push_back(index);
        }
        if (i+1 < content.size()) {
            temp = content[i+1] - '0';
            for (size_t j = 0; j < temp; j++) {
                disk.push_back(-1);
            }
        }
    }

    auto disk_copy = disk;
    size_t begin = 0;
    size_t end = disk.size()-1;
    while (begin < end) {
        if (disk[begin] == -1) {
            std::swap(disk[begin], disk[end]);
            end--;
        } else {
            begin++;
        }
    }

    std::cout << "Part 1: " << calc_checksum(disk) << std::endl;

    disk = std::move(disk_copy);
    int prev_id = -1;
    int prev_count = 0;
    for (int i = disk.size(); i > 0; i--) {
        int curr_id = disk[i-1];
        if (prev_id != curr_id) {
            if (prev_id != -1) {
                fill_in(disk, prev_id, prev_count, i);
            }
            prev_id = curr_id;
            prev_count = 1;
        } else {
            prev_count++;
        }
    }
    std::cout << "Part 2: " << calc_checksum(disk) << std::endl;
    return 0;
}
