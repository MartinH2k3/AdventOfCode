#include <algorithm>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <ranges>
#include <sstream>
#include <vector>
#include <string>
#include <set>
#include <ranges>
#include "helpers.h"

bool do_part2 = false;
size_t regA;
size_t regB;
size_t regC;
size_t instruction_pointer = 0;
std::vector<size_t> output;

size_t combo_operand(const size_t operand) {
    switch (operand) {
        case 4:
            return regA;
        case 5:
            return regB;
        case 6:
            return regC;
        case 7:
            std::cerr << "Invalid operand 7" << std::endl;
            return -1;
        default:
            return operand;
    }
}

void adv(const size_t operand) {
    regA >>= combo_operand(operand);
    instruction_pointer+=2;
}

void bxl(const size_t operand) {
    regB ^= operand;
    instruction_pointer+=2;
}

void bst(const size_t operand) {
    regB = combo_operand(operand) & 7;
    instruction_pointer+=2;
}

void jnz(const size_t operand) {
    if (regA != 0) {
        instruction_pointer = operand;
    } else {
        instruction_pointer+=2;
    }
}

void bxc(const size_t operand) {
    regB ^= regC;
    instruction_pointer+=2;
}

void out(const size_t operand) {
    instruction_pointer+=2;
    output.push_back(combo_operand(operand)&7);
    std::cout << (combo_operand(operand)&7) << ",";
}

void bdv(const size_t operand) {
    regB = regA >> combo_operand(operand);
    instruction_pointer+=2;
}

void cdv(const size_t operand) {
    regC = regA >> combo_operand(operand);
    instruction_pointer+=2;
}

void call_instruction(const size_t opcode, const size_t operand) {
    switch (opcode) {
        case 0:
            adv(operand);
            return;
        case 1:
            bxl(operand);
            return;
        case 2:
            bst(operand);
            return;
        case 3:
            jnz(operand);
            return;
        case 4:
            bxc(operand);
            return;
        case 5:
            out(operand);
            return;
        case 6:
            bdv(operand);
            return;
        case 7:
            cdv(operand);
            return;
        default:
            std::cerr << "Invalid opcode " << opcode << std::endl;
    }
}

std::vector<size_t> find_matching_a(const size_t target) {
    std::vector<size_t> options;
    for (size_t option = 0; option < 904; option++) { // 0b1110000111 is peak it can be
        if (const size_t mask = 7 | 7 << (option&7^3);
            (option & mask) != option) {
            continue;
        }
        if (((option ^ option>>(option&7 ^ 3))&7) == target) {
            options.push_back(option);
        }
    }
    return options;
}

bool valid_option(const size_t option, const size_t target) {
    return ((option ^ option>>(option&7 ^ 3))&7) == target;
}

bool valid_series(const std::vector<size_t>& instructions, size_t option, const size_t start_index) {
    for (size_t i = start_index; i < instructions.size(); i++) {
        if (!valid_option(option, instructions[i])) {
            return false;
        }
        option >>= 3;
    }
    return true;
}
int main() {
    std::ifstream file("bob.txt");
    std::string temp;
    file >> temp >> temp >> regA
        >> temp >> temp >> regB
        >> temp >> temp >> regC
        >> temp >> temp;
    const std::vector<size_t> instructions = split<uint64_t>(temp, ',');
    while (instruction_pointer < instructions.size()) {
        call_instruction(instructions[instruction_pointer], instructions[instruction_pointer+1]);
    }

    std::vector<std::vector<size_t>> options;
    for (size_t i = 0; i < 8; i++) {
        options.push_back(find_matching_a(i));
    }

    std::set<size_t> combinations;
    size_t i = 0;
    for (auto it = instructions.rbegin(); it != instructions.rend(); ++it) {
        std::set<size_t> new_combinations;
        for (auto option : options[*it]) {
            if (combinations.empty()) {
                new_combinations.insert(option);
                continue;
            }
            for (const auto combination : combinations) {
                if (valid_series(instructions, combination<<3 | option, instructions.size() - 1 - i)) {
                    new_combinations.insert(combination<<3 | option);
                }
            }
        }
        combinations = new_combinations;
        i++;
    }
    std::cout << std::endl;
    for (const auto combination : combinations) {
        std::cout << combination << std::endl;
    }
    return 0;
}