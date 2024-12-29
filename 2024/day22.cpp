#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <chrono>
size_t mask = 16777215;

size_t evolve(size_t num){
    num = ((num << 6)^num)&mask;
    num = ((num >> 5)^num)&mask;
    num = ((num << 11)^num)&mask;
    return num;
}

void fill_sequences(std::vector<int>& prices, std::map<int, int>& sequence_gains){
    std::vector<int> deltas;
    for (size_t i = 1; i < prices.size(); i++){
        deltas.push_back(prices[i] - prices[i-1]);
    }
    std::set<int> visited;
    for (int i = 3; i < deltas.size(); i++){
        int sequence_code = ((deltas[i-3]+10)<<24)+((deltas[i-2]+10)<<16)+((deltas[i-1]+10)<<8)+(deltas[i]+10);
        if (!visited.contains(sequence_code)){
            sequence_gains[sequence_code] += prices[i+1];
            visited.insert(sequence_code);
        }
    }
}

int main(){
    std::ifstream file("input.txt");
    size_t temp;
    size_t output = 0;
    std::map<int, int> sequence_gains;
    while(file >> temp){
        std::vector<int> prices;
        for (int i = 0; i < 2000; i++){
            prices.push_back(temp % 10);
            temp = evolve(temp);
        }
        prices.push_back(temp % 10);
        output += temp;
        fill_sequences(prices, sequence_gains);

    }
    std::cout << "Part 1: " << output << std::endl;
    int max = 0;
    for (const auto& [key, value] : sequence_gains){
        if (value > max){
            max = value;
        }
    }
    std::cout << "Part 2: " << max << std::endl;
    return 0;
}