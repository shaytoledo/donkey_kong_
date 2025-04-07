#pragma once

#include <string>
#include <iostream>
#include <windows.h>
#include <cstring>
#include <filesystem>
#include <fstream>
#include<conio.h>
#include <string>
#include "Utils.h"
#include "Screen.h"

class Board {
private:
	// Screen colors specification
	Screen& screen;
	Colors DEFAULT_COLOR = screen.getColor("board");

	// Characters that define the board blocks
	static constexpr char blocksChars[4] = { '=', '<', '>', 'Q' }; // Types of chars that limit movement
	static constexpr char airChar = ' ';
	static constexpr char ladderChar = 'H';
	static constexpr char ghostChar = 'x';
	static constexpr char hammerChar = 'p';
	static constexpr char superGhostChar = 'X';

	// Dimensions of the board
	static constexpr int MAX_X = 80;  // Maximum width of the board (from lab exercise 3)
	static constexpr int MAX_Y = 25;  // Maximum height of the board (from lab exercise 3)
	static const int MIN_COORD = 0;  // Minimum coordinate axis value (used for validation)

	// Coordinates for objects
	Coord paulinaPos = { 2,29 };
	Coord marioStartPos = { 0,0 };
	Coord hammerPos = { -1,-1 }; //sign for no hammer
	Coord liveAndScorePos = { 1,12 }; 	// Coordinates for displaying Mario's lives and total Score
	std::vector<Coord> ghostsStartPos;
	std::vector<Coord> superGhostsStartPos;
	std::vector<Coord> barrelsStartPos;
	
	char originalBoard[MAX_Y][MAX_X + 1];
	char currentBoard[MAX_Y][MAX_X + 1]; // The current state of the board (+1 for null terminator)
	std::string currLevel;

public:
	Board() = default;

	// Constructor for the boardhr
	Board(Screen& screen, std::string selectedBoard) : 
		screen(screen) , currLevel(selectedBoard)  { reset(); }

	// Get the current level file name
	std::string getCurrLevel() const {return currLevel; }

	void setCurrLevel(std::string level) { currLevel = level; }

	// Reset the board to its original state
	void reset(); // Taken from lab exercise 3

	// Print the board to the console
	void print() const; // Taken from lab exercise 3

	// Get the character at a specific position on the board
	char getChar(Coord pos) const { return currentBoard[pos.y][pos.x]; } // Taken from lab exercise 3
	
	// Set the character at a specific position on the board
	void setChar(Coord pos, char c) { currentBoard[pos.y][pos.x] = c; }

	// Check if a given coordinate is within valid boundaries
	bool isValidCoord(Coord pos) const;

	// Check if a given position is "air" (empty space)
	char isAir(Coord pos) const { return (getChar(pos) == airChar); }

	// Check if a given position contains a ladder
	char isLadder(Coord pos) const { return getChar(pos) == ladderChar; }

	// Check if a given position is a floor
	bool isFloor(Coord pos) const;

	// Check if a position is at the edge of the screen
	bool isEdge(Coord pos) const { return (getChar(pos) == blocksChars[3]); }

	// Check if a position is a hammer
	bool isHammer(Coord pos) const { return (getChar(pos) == hammerChar); }

	// Check if a given position is a straight floor ('=' character)
	bool isStrightFloor(Coord pos) const { return getChar(pos) == blocksChars[0]; }
	
	// check it a given position is a ghost
	bool isGhost(Coord pos) const { return getChar(pos) == ghostChar; }

	// Print Mario's remaining lives to the screen
	void printLivesAndScore(int lives, int score) const;

	// Check if Mario has reached Paulina's position
	bool isMarioReachPaulina(Coord pos) const { return (pos == paulinaPos); }
	
	// set the current board
	bool setBoard(const bool flag = false);

	// Get the position Mario starts at
	Coord getMarioStartPos() const { return marioStartPos; }

	// Get the start position of Pailine
	Coord getPaulinaPos() const { return paulinaPos; }
	
	// Get the start position of the Hammer
	Coord getHammerPos() const { return hammerPos; }
	
	// Get the character that represents the hammer
	char getHammerChar() const { return hammerChar; }
	
	// Return the posations of donky-kongs (where the barrels start).
	std::vector<Coord>& getBarrelsStartPos() { return barrelsStartPos; }
	
	// Return the posation of all of the ghosts
	std::vector<Coord>& getGhostsStartPos()  { return ghostsStartPos; }
	
	// Return the position of all of the SuperGhosts
	std::vector<Coord>& getSuperGhostsStartPos() { return superGhostsStartPos; }

	//Check if the board is valid 
	bool isValidBoard(const std::string fileName, bool flag) const;

	//Check if the file is good and if the board in the file valid 
	bool checkCurrBoard(std::ifstream& screen_file, bool flag) const;
	
	//Report abot which Elements are miss
	void checkAndReportMissingElements(int& paulinaCounter, int& marioCounter, int& kongCounter, int& liveAndScoreCounter) const;

	//Initializes the positions of objects on the board.
	void processSpecialCharacters(char& c, int curr_col, int curr_row) ;

	// Add frame to the board (Q)
	void addFrame();

	// Fill the line with some value and add \n
	void fill(char* dest, int count, char val, int curr_row) ;

	//Print specific character
	void printChar(char charToErase, Coord charPos) {
		setChar(charPos, charToErase);
		gotoxy(charPos);
		screen.coloredCharPrint(charToErase, DEFAULT_COLOR);
	}

	//Eraes secific character
	void eraseChar(Coord charPos) {
		printChar(airChar, charPos);
	}

	//Print hammer
	void printHammer() {
		printChar(hammerChar, hammerPos);
	}

	//Eraes hammer
	void eraseHammer() {
		eraseChar(hammerPos);
	}
};

