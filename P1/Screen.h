#pragma once

#include <iostream>
#include "string"
#include <map>
#include "utils.h"
#include <windows.h>

class Screen {
private:

    static constexpr int SCREEN_WIDTH = 25, SCREEN_HIGHET = 80;
    static constexpr Coord SCREEN_CENTER = { 31, 10 };
    

    bool isColorsEnabled = false;
	bool isSilentMode = false;
    static constexpr Colors DEFAULT_COLOR= Colors::WHITE;

	std::map<std::string, Colors> colorMap = { // was taken from perplexity: "how to create a map in c++"
        {"menu", Colors::DARK_GREEN},
		{"@", Colors::GREEN}, // mario
		{"board", Colors::DARK_AZURE},
        {"O", Colors::YELLOW}, // barrel
        {"x", Colors::DARK_BLUE}, // ghost
        {"paulina", Colors::PINK},
        {"lives", Colors::RED},
		{"explotion", Colors::RED},
		{"gameOver", Colors::RED},
		{"win", Colors::GREEN},
        {"invalidInput", Colors::RED}
    };

public:

    Screen(bool isSilentMode = false) : isSilentMode(isSilentMode) {};
	
    // Set the boolean value of isColorsEnabled
	void setSilentMode(bool flag) { isSilentMode = flag; }

    // Switch the boolean value of isColorsEnabled
    void switchColorsStatus() { isColorsEnabled = !isColorsEnabled; }

    // Set the console text colors
    void setColor(Colors color = Colors::WHITE) {  // was taken from instructions bonus section
        WORD colorCode = colorToCode(color);
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, colorCode);
    }

    // Set a chars color map
    void setCharsColorMap(std::map<std::string, Colors> charsColorMap) { charsColorMap = charsColorMap; }

    // Function to reset the console color
    void resetColor() { setColor(); }

    // Convert Colors data member to its integer value
    WORD colorToCode(Colors color) {  // was taken from perplexity: "how to convert enum class value to int"
        return static_cast<WORD>(color);
    }
	// Print a colored char to the console
    void coloredCharPrint(char c, Colors color) {
        if (!isSilentMode) {
            if (!isColorsEnabled) {
                color = Colors::WHITE;
            }
            setColor(color);
            std::cout << c;
            resetColor();
        }
    }
	// Print a colored string to the console
    void coloredStrPrint(std::string text, Colors color = Colors::WHITE, bool toResetColor = true) {
        if (!isSilentMode) {
            if (!isColorsEnabled) { 
                color = Colors::WHITE; }
            setColor(color);
            std::cout << text;
            if (toResetColor) { resetColor(); }  // set to false when using inside iterations of the same color
        }
    };

    // Clear the console
    void clear() { system("cls"); }

    // Get the color based on the input criteria
	Colors getColor(const std::string& criteria) const { // was taken from perplexity: "how to find a key in a map"
        auto it = colorMap.find(criteria);
        if (it != colorMap.end()) {
            return it->second;
        }
        else {
            return DEFAULT_COLOR;
        }
    }
    
    // Overloaded getColor function that accepts a char
    Colors getColor(char criteria) const {
        return getColor(std::string(1, criteria));
    }
};
