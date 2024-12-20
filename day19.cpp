#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <regex>

bool begins_with(const std::string& str, const std::string& substr){
    return str.substr(0, substr.size()) == substr;
}
size_t possibilities(const std::string& substring, std::map<std::string, size_t>& combinations, const std::set<std::string>& parts){
    if (combinations.contains(substring)){
        return combinations[substring];
    }
    size_t result = 0;
    for (const auto& part : parts){
        if (substring == part){
            result++;
            continue;
        }
        if (begins_with(substring, part)){
            result += possibilities(substring.substr(part.size()), combinations, parts);
        }
    }
    combinations[substring] = result;
    return result;
}

int main(){
    std::ifstream file("input.txt");
    std::string line;
    std::map<std::string, size_t> combinations;
    getline(file, line);
    std::string regex = line;
    regex.erase(std::remove(regex.begin(), regex.end(), ' '), regex.end());
    std::replace(regex.begin(), regex.end(), ',', '|');
    regex = "(" + regex + ")+";
    std::replace(line.begin(), line.end(), ',', ' ');
    std::istringstream buffer(line);
    std::string temp;
    std::set<std::string> parts;
    while (buffer >> temp){
        parts.insert(temp);
    }

    size_t part1 = 0;
    size_t part2 = 0;
    while (getline(file, line)){
        // if regex matches part1++
        std::regex re(regex);
        std::smatch match;
        if (!std::regex_match(line, match, re)){
            continue;
        }
        part1++;
        part2 += possibilities(line, combinations, parts);
    }
    std::cout << part1 << std::endl;
    std::cout << part2 << std::endl;
    return 0;
}