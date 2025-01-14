#ifndef HELPERS_H
#define HELPERS_H
#include <compare>
#include <map>
#include <ranges>
#include <vector>

struct Point {
    int x;
    int y;
    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}
    Point operator+(const Point& other) const {
        return {x + other.x, y + other.y};
    }
    Point operator-(const Point& other) const {
        return {x - other.x, y - other.y};
    }
    Point operator*(const int scalar) const {
        return {x * scalar, y * scalar};
    }
    Point operator/(const int scalar) const {
        return {x / scalar, y / scalar};
    }
    Point operator%(const Point other) const {
        return {x % other.x, y % other.y};
    }
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Point& other) const {
        return x != other.x || y != other.y;
    }
    auto operator<=>(const Point& other) const {
        if (const auto cmp = y <=> other.y; cmp != 0) {
            return cmp;
        }
        return x <=> other.x;
    }
    bool within(const int width, const int height) const {
        return x >= 0 && x < width && y >= 0 && y < height;
    }
    bool within(const size_t width, const size_t height) const {
        return x < width && y < height;
    }
    std::vector<Point> neighbors(size_t width, size_t height) const;
};
inline std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}
enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

inline std::map<Direction, Point> directions = {
    {Direction::UP, {0, -1}},
    {Direction::DOWN, {0, 1}},
    {Direction::LEFT, {-1, 0}},
    {Direction::RIGHT, {1, 0}}
};

inline Direction turn_left(const Direction dir) {
    switch (dir) {
        case Direction::UP:
            return Direction::LEFT;
        case Direction::DOWN:
            return Direction::RIGHT;
        case Direction::LEFT:
            return Direction::DOWN;
        case Direction::RIGHT:
            return Direction::UP;
    }
    throw std::runtime_error("Invalid direction");
}
inline Direction turn_right(const Direction dir) {
    switch (dir) {
        case Direction::UP:
            return Direction::RIGHT;
        case Direction::DOWN:
            return Direction::LEFT;
        case Direction::LEFT:
            return Direction::UP;
        case Direction::RIGHT:
            return Direction::DOWN;
    }
    throw std::runtime_error("Invalid direction");
}
inline Direction reverse(const Direction dir) {
    switch (dir) {
        case Direction::UP:
            return Direction::DOWN;
        case Direction::DOWN:
            return Direction::UP;
        case Direction::LEFT:
            return Direction::RIGHT;
        case Direction::RIGHT:
            return Direction::LEFT;
    }
    throw std::runtime_error("Invalid direction");
}
inline std::ostream& operator<<(std::ostream& os, const Direction dir) {
    switch (dir) {
        case Direction::UP:
            os << "UP";
            break;
        case Direction::DOWN:
            os << "DOWN";
            break;
        case Direction::LEFT:
            os << "LEFT";
            break;
        case Direction::RIGHT:
            os << "RIGHT";
            break;
    }
    return os;
}

inline std::vector<Point> Point::neighbors(const size_t width, const size_t height) const {
    std::vector<Point> neighbors;
    for (const auto& [_, dir] : directions) {
        if (Point neighbor = *this + dir;
            neighbor.within(width, height)) {
            neighbors.push_back(neighbor);
        }
    }
    return neighbors;
}

inline int power(int base, int exp) {
    int result = 1;
    while (exp) {
        if (exp & 1) {
            result *= base;
        }
        exp >>= 1;
        base *= base;
    }
    return result;
}

template <typename T>
std::vector<T> split(std::string input, const char delimiter) {
    std::ranges::replace(input.begin(), input.end(), delimiter, ' ');
    std::istringstream iss(input);
    std::vector<T> tokens;
    for (T token; iss >> token;) {
        tokens.push_back(token);
    }
    return tokens;
}

template <typename T>
std::vector<T> split(std::string input, const std::string& delimiters) {
    for (const auto& delimiter : delimiters) {
        std::ranges::replace(input.begin(), input.end(), delimiter, ' ');
    }
    std::istringstream iss(input);
    std::vector<T> tokens;
    for (T token; iss >> token;) {
        tokens.push_back(token);
    }
    return tokens;
}

class Bitmap{
private:
    size_t length; // using word length instead of size, so that we can use size() method
    std::vector<size_t> data;

    class BitReference {
    private:
        size_t& word;     // Reference to the 64-bit word in the data
        size_t bitIndex;  // Index of the bit within the word

    public:
        BitReference(size_t& word, size_t bitIndex)
                : word(word), bitIndex(bitIndex) {}

        // Implicit conversion to bool for reading
        operator bool() const {
            return word & (1ULL << bitIndex);
        }

        // Assignment for writing
        BitReference& operator=(bool value) {
            if (value) {
                word |= (1ULL << bitIndex); // Set the bit
            } else {
                word &= ~(1ULL << bitIndex); // Clear the bit
            }
            return *this;
        }
    };
public:
    Bitmap() = default;
    explicit Bitmap(size_t size){
        data = std::vector<size_t>(size/64 + (size%64 > 0), 0);
        this->length = size;
    }
    Bitmap(std::vector<bool> bitmap){
        data = std::vector<size_t>(bitmap.size()/64 + (bitmap.size()%64 > 0), 0);
        for (size_t i = 0; i < bitmap.size(); i++){
            if (bitmap[i]){
                data[i/64] |= 1 << (i%64);
            }
        }
        length = bitmap.size();
    }
    // Assignable operator[]
    BitReference operator[](size_t index) {
        return BitReference(data[index / 64], index % 64);
    }

    // Const operator[] for read-only access
    bool operator[](size_t index) const {
        return data[index / 64] & (1ULL << (index % 64));
    }
    Bitmap& operator++(){
        for (size_t i = 0; i < data.size(); i++){
            if (data[i] == SIZE_MAX){
                data[i] = 0;
            } else {
                data[i]++;
                break;
            }
        }
        return *this;
    }
    operator std::vector<bool>() const {
        std::vector<bool> bitmap;
        for (size_t i = 0; i < data.size(); i++){
            for (size_t j = 0; j < 64; j++){
                bitmap.push_back(data[i] & (1 << j));
            }
        }
        return bitmap;
    }

    size_t true_count(){
        size_t count = 0;
        for (size_t i = 0; i < data.size(); i++){
            count += __builtin_popcountll(data[i]);
        }
        return count;
    }

    size_t size() const{
        return length;
    }
    bool is_overflown(){
        return data[length / 64] & (1 << (length % 64));
    }
};

std::string to_binary_string(int64_t number){
    std::string output;
    while (number){
        output.push_back((number&1) + '0');
        number >>= 1;
    }
    std::reverse(output.begin(), output.end());
    return output;
}

#endif //HELPERS_H
