#include <algorithm>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <set>
#include <ranges>
#include "helpers.h"

bool do_part2 = false;
uint64_t regA;
uint64_t regB;
uint64_t regC;
uint64_t instruction_pointer = 0;

uint64_t combo_operand(const uint64_t operand) {
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

void adv(const uint64_t operand) {
    regA >>= combo_operand(operand);
    instruction_pointer+=2;
}

void bxl(const uint64_t operand) {
    regB ^= operand;
    instruction_pointer+=2;
}

void bst(const uint64_t operand) {
    regB = combo_operand(operand) & 7;
    instruction_pointer+=2;
}

void jnz(const uint64_t operand) {
    if (regA != 0) {
        instruction_pointer = operand;
    } else {
        instruction_pointer+=2;
    }
}

void bxc(const uint64_t operand) {
    regB ^= regC;
    instruction_pointer+=2;
}

void out(const uint64_t operand) {
    instruction_pointer+=2;
    std::cout << (combo_operand(operand)&7) << ",";
}

void bdv(const uint64_t operand) {
    regB = regA >> combo_operand(operand);
    instruction_pointer+=2;
}

void cdv(const uint64_t operand) {
    regC = regA >> combo_operand(operand);
    instruction_pointer+=2;
}

void call_instruction(const uint64_t opcode, const uint64_t operand) {
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

int main() {
    std::ifstream file("bob.txt");
    std::string temp;
    file >> temp >> temp >> regA
        >> temp >> temp >> regB
        >> temp >> temp >> regC
        >> temp >> temp;
    const std::vector<uint64_t> instructions = split<uint64_t>(temp, ',');
    while (instruction_pointer < instructions.size()) {
        call_instruction(instructions[instruction_pointer], instructions[instruction_pointer+1]);
    }
    return 0;
}