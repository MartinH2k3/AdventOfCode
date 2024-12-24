#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <algorithm>
#include "helpers.h"

enum class operation{
    AND,
    OR,
    XOR
};
void fix_dependencies(int key);
void evaluate(int key);
void evaluate(const std::string& problem);

std::map<int, bool> values;                             // boolean values for keys
std::map<int, std::vector<int>> resolved_dependencies;  // values the key has to evaluate from
std::map<int, operation> waiting_operation;             // operation which is to be applied when done resolved_dependencies
std::map<int, std::vector<int>> dependants;             // which values are resolved_dependencies for the key

int encode(std::string input){
    int result = 0;
    for (size_t i = 0; i < input.size(); i++){
        result |= input[i] << ((input.size()-1-i)*8);
    }
    return result;
}

std::string decode(int encoded){
    std::string result;
    for (int i = 2; i >= 0; i--){
        result.push_back((char)(encoded >> (i*8)));
    }
    return result;
}

void fix_dependencies(int key){
    for (const auto& dependent : dependants[key]){
        resolved_dependencies[dependent].push_back(key);
        evaluate(dependent);
    }
    dependants.erase(key);
}

void evaluate(int key){
    if (resolved_dependencies[key].size() < 2)
        return;
    int key1 = resolved_dependencies[key][0];
    int key2 = resolved_dependencies[key][1];

    if (waiting_operation[key] == operation::AND)
        values[key] = values[key1] && values[key2];
    else if (waiting_operation[key] == operation::OR)
        values[key] = values[key1] || values[key2];
    else if (waiting_operation[key] == operation::XOR)
        values[key] = values[key1] ^ values[key2];
    fix_dependencies(key);
}

void evaluate(const std::string& problem){
    std::istringstream iss(problem);
    std::string key1, op, key2, result;
    iss >> key1 >> op >> key2 >> result >> result;
    int encoded_key1 = encode(key1);
    int encoded_key2 = encode(key2);
    int encoded_result = encode(result);
    if (values.contains(encoded_key1) && values.contains(encoded_key2)) {
        if (op == "AND")
            values[encoded_result] = values[encoded_key1] && values[encoded_key2];
        else if (op=="OR")
            values[encoded_result] = values[encoded_key1] || values[encoded_key2];
        else if (op=="XOR")
            values[encoded_result] = values[encoded_key1] ^ values[encoded_key2];
        fix_dependencies(encoded_result);
        return;
    }
    // add to resolved_dependencies list
    if (values.contains(encoded_key1)){
        resolved_dependencies[encoded_result].push_back(encoded_key1);
    } else {
        dependants[encoded_key1].push_back(encoded_result);
    }
    if (values.contains(encoded_key2)){
        resolved_dependencies[encoded_result].push_back(encoded_key2);
    } else {
        dependants[encoded_key2].push_back(encoded_result);
    }
    waiting_operation[encoded_result] = op == "AND" ? operation::AND : op == "OR" ? operation::OR : operation::XOR;
}

int main() {
    std::ifstream file("input.txt");
    std::string line;
    while (std::getline(file, line)){
        if (line.empty()){
            break;
        }
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream iss(line);
        std::string key;
        bool value;
        iss >> key >> value;
        values[encode(key)] = value;
    }
    while (std::getline(file, line)){
        evaluate(line);
    }
    int64_t z = 0, y = 0, x = 0;
    for (auto it = values.rbegin(); it != values.rend(); ++it) {
        auto [key, value] = *it;
        if ((key>>16)=='z') {
            z <<= 1;
            z |= value;
        }
        else if ((key>>16)=='y') {
            y <<= 1;
            y |= value;
        }
        else if ((key>>16)=='x') {
            x <<= 1;
            x |= value;
        }
        else break;
        //std::cout << decode(key) <<  ": " << value << std::endl;
    }
    std::cout << "Part 1: " << z << std::endl;
    // print x, y, expected sum, z. You will see which znn is incorrect
    std::cout << "x:  " << to_binary_string(x) << std::endl;
    std::cout << "y:  " << to_binary_string(y) << std::endl;
    std::cout << "e: " << to_binary_string(x+y) << std::endl;
    std::cout << "z: " << to_binary_string(z) << std::endl;
    std::cout << "   ";
    for (int i = to_binary_string(z).size()-1; i >= 0; i--){
        std::cout << i%10;
    }
    std::cout << std::endl << "   ";
    for (int i = to_binary_string(z).size()-1; i >= 0; i--){
        std::cout << (char)(i%10 ? ' ' : i/10 ? '0'+i/10 : ' ');
    }
    // zn is supposed to be an^bn
    // an is (yn^xn)
    // bn is (yn-1 & xn-1)|(an-1 & bn-1)
    // start from the lowest bit, if something isn't correct, fix it.
    // Then rerun the code to see the next lowermost incorrect bit and fix again.
    return 0;
}


