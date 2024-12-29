#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <algorithm> // for std::find, std::remove
#include "helpers.h"

constexpr int size = 26*26;
std::vector<std::set<int>> adjacency(size);

int encode(char a, char b) {
    return (a - 'a') * 26 + (b - 'a');
}

std::pair<char, char> decode(int encoded) {
    return {static_cast<char>(encoded / 26 + 'a'),
            static_cast<char>(encoded % 26 + 'a')};
}

bool starts_with_t(int encoded) {
    // 't' == 'a' + 19
    return (encoded / 26) == 19;
}

int count_t_triangles() {
    int output = 0;
    for (int i = 0; i < size; i++){
        for (int j = i; j < size; j++){
            for (int k = j; k < size; k++){
                // If any of i, j, k starts with 't' and they form a triangle
                if (starts_with_t(i) || starts_with_t(j) || starts_with_t(k)){
                    if (   adjacency[i].contains(j)
                           && adjacency[j].contains(k)
                           && adjacency[k].contains(i)) {
                        output++;
                    }
                }
            }
        }
    }
    return output;
}

std::vector<int> best_clique;
int best_size = 0;

void max_clique_helper(std::vector<int> R, std::vector<int> P, std::vector<int> X) {
    if (P.empty() && X.empty()) {
        if ((int)R.size() > best_size) {
            best_size = (int)R.size();
            best_clique = R;
        }
        return;
    }

    int pivot = -1;
    if (!P.empty()) {
        pivot = P[0];
    }

    std::set<int> pivot_neighbors;
    if (pivot != -1) {
        pivot_neighbors = adjacency[pivot];
    }

    std::vector<int> candidates;
    for (int v : P) {
        if (pivot == -1 || !pivot_neighbors.contains(v)) {
            candidates.push_back(v);
        }
    }

    for (int v : candidates) {
        std::vector<int> Rv = R;
        Rv.push_back(v);

        std::vector<int> Pnext, Xnext;
        for (int w : P) {
            if (adjacency[v].contains(w)) {
                Pnext.push_back(w);
            }
        }
        for (int w : X) {
            if (adjacency[v].contains(w)) {
                Xnext.push_back(w);
            }
        }

        max_clique_helper(Rv, Pnext, Xnext);

        P.erase(std::find(P.begin(), P.end(), v));
        X.push_back(v);
    }
}

void print_max_clique() {
    best_clique.clear();
    best_size = 0;

    std::vector<int> R, P, X;
    for (int i = 0; i < size; i++){
        if (!adjacency[i].empty()) {
            P.push_back(i);
        }
    }

    max_clique_helper(R, P, X);

    // Print the maximum clique found
    std::cout << "Nodes in max clique: ";
    for (int node : best_clique) {
        auto [c1, c2] = decode(node);
        std::cout << c1 << c2 << ",";
    }
    std::cout << std::endl;
}

int main() {
    std::ifstream file("input.txt");
    std::string line;

    while (std::getline(file, line)) {
        std::replace(line.begin(), line.end(), '-', ' ');
        std::string temp;
        int a, b;
        std::istringstream iss(line);
        iss >> temp;
        a = encode(temp[0], temp[1]);
        iss >> temp;
        b = encode(temp[0], temp[1]);

        adjacency[a].insert(b);
        adjacency[b].insert(a);
    }

    std::cout << "Part 1: " << count_t_triangles() << std::endl;
    std::cout << "Part 2: ";
    print_max_clique();
    return 0;
}
