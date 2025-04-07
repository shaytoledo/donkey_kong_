#pragma once
#include <string.h>
#include <filesystem>
#include <iostream>
#include <windows.h>
#include <functional>

// Colors enum class for overall usage
enum class Colors {
    DARK_BLUE = 1,
    DARK_GREEN = 2,
    DARK_AZURE = 3,
    DARK_RED = 4,
    DARK_PINK = 5,
    DARK_YELLOW = 6,
    DARK_WHITE = 7,
    DARK_GREY = 8,
    BLUE = 9,
    GREEN = 10,
    AZURE = 11,
    RED = 12,
    PINK = 13,
    YELLOW = 14,
    WHITE = 15,
    BLACK = 16
};

// Direction struct of Entity for overall usage
struct Direction {
    int x;
    int y;

	bool operator==(const Direction& other) const {
		return x == other.x && y == other.y;
	}
	Direction operator*(int i) const {
		return { x * i, y * i };
	}
    Direction operator!() const {
        if (*this == UP) return DOWN;
        if (*this == DOWN) return UP;
        if (*this == LEFT) return RIGHT;
        if (*this == RIGHT) return LEFT;
        return STAY;
    }
   
    static const Direction STAY;
    static const Direction UP;
    static const Direction DOWN;
    static const Direction LEFT;
    static const Direction RIGHT;
};

// coordinates struct of Point for overall usage
struct Coord {
    int x;
    int y;

    // Overload the == operator
    bool operator==(const Coord& other) const {
        return x == other.x && y == other.y;
    }

    // Overload the += operator
	Coord& operator+=(const Direction& dir) { // was taken from chatGpt: "how to overload the += operator between two structs"
        this->x += dir.x;
        this->y += dir.y;
        return *this; 
    }
    // Overload the != operator
    bool operator!=(const Coord& other) const {
        return x != other.x || y != other.y;
    }
    // Overload the + operator
    Coord operator+(const Direction& dir) {
		return { x + dir.x, y + dir.y };
    }
    // Overload the - operator
    Coord operator-(const Coord& dir) {
		return { x - dir.x, y - dir.y };
    }
};

// Custom hash function for Coord
namespace std { // was taken from chatGpt: "how to use Coord as a key in unordered_map"
    template <>
    struct hash<Coord> {
        std::size_t operator()(const Coord& coord) const {
            return std::hash<int>()(coord.x) ^ (std::hash<int>()(coord.y) << 1);
        }
    };
}

// Go to the specific position in the console
void gotoxy(Coord pos);

// Show the cursor on the console
void ShowConsoleCursor(bool showFlag);

// Find files by predix and suffix  
std::vector <std::string> findFiles(std::string prefix, std::string ext);

// Change extension of a file to a new given extension
std::string changeExtension(const std::string& filename, const std::string& newExtension);