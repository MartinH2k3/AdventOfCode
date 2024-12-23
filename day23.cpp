#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

constexpr int size = 26*26;
std::vector<std::vector<bool>> adjacency_matrix(size, std::vector<bool>(size, false));

int encode(char a, char b){
    return (a - 'a')*26 + (b - 'a');
}

bool starts_with_t(int encoded){
    return encoded / 26 == 19;
}

int count_t_triangles(){
    int output = 0;
    for (int i = 0; i < size; i++){
        for (int j = i; j < size; j++){
            for (int k = j; k < size; k++){
                if (starts_with_t(i) || starts_with_t(j) || starts_with_t(k)){
                    if (adjacency_matrix[i][j] && adjacency_matrix[j][k] && adjacency_matrix[k][i]){
                        output++;
                    }
                }
            }
        }
    }
    return output;
}

int main(){
    std::ifstream file("input.txt");
    std::string line;
    while (std::getline(file, line)){
        std::replace(line.begin(), line.end(), '-', ' ');
        std::string temp;
        int a, b;
        std::istringstream iss(line);
        iss >> temp; a = encode(temp[0], temp[1]);
        iss >> temp; b = encode(temp[0], temp[1]);
        adjacency_matrix[a][b] = adjacency_matrix[b][a] = true;
    }
    std::cout << count_t_triangles() << std::endl;
    return 0;
}