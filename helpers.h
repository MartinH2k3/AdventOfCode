#ifndef HELPERS_H
#define HELPERS_H
#include <compare>
#include <map>

struct Point {
    int x;
    int y;
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

#endif //HELPERS_H