#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include "utils.h"
#include "Screen.h"


class Menu {
private:

	// Screen specifications
	Screen& screen;
	bool isColorsEnabled = false;
	Colors DEFAULT_COLOR = screen.getColor("menu");

	// User input specifications
	char _userChoice = NULL;
	int _userChoiceInt = -1;

	static constexpr char BAD_INPUT_CHAR = ' ';
	static constexpr int MAX_INPUT_SIZE = 1;

	//Board selection 
	std::string selectedBoard;

	//All files number (for the levels)
	std::vector<std::string> filesNamesSorted;

	// Instructions
	static constexpr int NUM_OF_INSTRUCTIONS_PAGES = 3, TIME_BETWEEN_PAGES = 800, TIME_OF_START_GAME = 1500;
	
	std::string instructions[NUM_OF_INSTRUCTIONS_PAGES] = {
		"+------------------------------------------------------------------------------+\n"
		"|                                                                              |\n"
		"|                     Welcome to the World of Donkey Kong!                     |\n"
		"|                                                                              |\n"
		"|  Your mission : Save the damsel in distress (Pauline) from the clutches of   |\n"
		"|  the mischievous Donkey Kong.                                                |\n"
		"|                                                                              |\n"
		"|  Dodge barrels, climb ladders, and reach the top to rescue her!              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|  Press n for next, b for back and m for menu                                 |\n"
		"|                                                                              |\n"
		"|  >>                                                                          |\n"
		"+------------------------------------------------------------------------------+",
		"+------------------------------------------------------------------------------+\n"
		"|                                                                              |\n"
		"|                              How to Play                                     |\n"
		"|                                                                              |\n"
		"|  You control Jumpman (Mario), and your task is to:                           |\n"
		"|                                                                              |\n"
		"|    1. Navigate platforms.                                                    |\n"
		"|    2. Climb ladders to ascend to higher levels.                              |\n"
		"|    3. Avoid barrels thrown by Donkey Kong.                                   |\n"
		"|    4. Reach Pauline at the top of the level.                                 |\n"
		"|                                                                              |\n"
		"|  Press n for next, b for back and m for menu                                 |\n"
		"|                                                                              |\n"
		"|  >>                                                                          |\n"
		"+------------------------------------------------------------------------------+\n",
		"+------------------------------------------------------------------------------+\n"
		"|                                                                              |\n"
		"|                                Controls                                      |\n"
		"|                                                                              |\n"
		"|  w or W: Move up / Jump (Climb ladders)                                      |\n"
		"|  d or D: Move right                                                          |\n"
		"|  a or A: Move left                                                           |\n"
		"|  x or X: Move down (Descend ladders)                                         |\n"
		"|  s or S: Stay                                                                |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|  Press n for next, b for back and m for menu                                 |\n"
		"|                                                                              |\n"
		"|  >>                                                                          |\n"
		"+------------------------------------------------------------------------------+\n"
	}; // The sentences were drafted by ChatGPT.

	std::string mainMenuPage =
		"+------------------------------------------------------------------------------+\n"
		"|                                                                              |\n"
		"|                               Main Menu                                      |\n"
		"|                                                                              |\n"
		"|  (1) Start a new game                                                        |\n"
		"|  (2) Enable screen colors (Disabled)                                         |\n"
		"|  (8) Present instructions and keys                                           |\n"
		"|  (9) EXIT                                                                    |\n"
		"|  >>                                                                          |\n"
		"|                                                                              |\n"
		"+------------------------------------------------------------------------------+";

		std::string goodByePage =
		"+------------------------------------------------------------------------------+\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                            thank you! bye bye                                |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"+------------------------------------------------------------------------------+";

	std::string pauseGamePage =
		"+------------------------------------------------------------------------------+\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                               Game Pause!                                    |\n"
		"|                                                                              |\n"
		"|                           Click ESC to return                                |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"+------------------------------------------------------------------------------+";

	std::string invalidInputPage =
		"+------------------------------------------------------------------------------+\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                        invalid input, try again.                             |\n"
		"|                                                                              |\n"
		"|                               \\_(0 ^ 0)_/                                    |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"+------------------------------------------------------------------------------+";
	std::string nextLevel =
		" +-----------------------------------------------------------------------------+\n"
		" |                                                   /$$                       |\n "
		"|                                                 | $$                        |\n "
		"|                  /$$$$$$$   /$$$$$$  /$$   /$$ /$$$$$$                      |\n "
		"|                 | $$__  $$ /$$__  $$|  $$ /$$/|_  $$_/                      |\n "
		"|                 | $$  \\ $$| $$$$$$$$ \\  $$$$/   | $$                        |\n "
		"|                 | $$  | $$| $$_____/  >$$  $$   | $$ /$$                    |\n "
		"|                 | $$  | $$|  $$$$$$$ /$$/\\  $$  |  $$$$/                    |\n "
		"|                 |__/  |__/ \\_______/|__/  \\__/   \\___/ v                    |\n "
		"|                                                                             |\n "
		"|                                                                             |\n "
		"|                                                                             |\n "
		"|                  /$$                               /$$ /$$                  |\n "
		"|                 | $$                              | $$| $$                  |\n "
		"|                 | $$  /$$$$$$  /$$    /$$ /$$$$$$ | $$| $$                  |\n "
		"|                 | $$ /$$__  $$|  $$  /$$//$$__  $$| $$| $$                  |\n "
		"|                 | $$| $$$$$$$$ \\  $$/$$/| $$$$$$$$| $$|__/                  |\n "
		"|                 | $$| $$_____/  \\  $$$/ | $$_____/| $$                      |\n "
		"|                 | $$|  $$$$$$$   \\  $/  |  $$$$$$$| $$ /$$                  |\n "
		"|                 |__/ \\_______/    \\_/    \\_______/|__/|__/                  |\n "
		"+-----------------------------------------------------------------------------+";


														 
	std::string winGamePage = // was generated by: https://www.asciiart.eu/text-to-ascii-art
		"+------------------------------------------------------------------------------+\n"
		"|                                                                              |\n"
		"| /$$     /$$                                                          / $$    |\n"
		"||  $$   /$$/                                                          | $$    |\n"
		"| \\  $$ /$$//$$$$$$   /$$   /$$       /$$  /$$  /$$  /$$$$$$  /$$$$$$$ | $$    |\n"
		"|  \\  $$$$//$$__  $$ | $$  | $$      | $$ | $$ | $$ /$$__  $$| $$__  $$| $$    |\n"
		"|   \\  $$/| $$  \\ $$ | $$  | $$      | $$ | $$ | $$| $$  \\ $$| $$  \\ $$|__/    |\n"
		"|    | $$ | $$  | $$ | $$  | $$      | $$ | $$ | $$| $$  | $$| $$  | $$        |\n"
		"|    | $$ |  $$$$$$ /|  $$$$$$/      |  $$$$$/$$$$/|  $$$$$$/| $$  | $$ /$$    |\n"
		"|    | __ / \\______/ \\______ /        \\_____/\\___/  \\______/ |__/  |__/|__/    |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                     Press any key to return to the menu                      |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"+------------------------------------------------------------------------------+";

	std::string gameOverPage = // was generated by: https://www.asciiart.eu/text-to-ascii-art
		"+------------------------------------------------------------------------------+\n"
		"|                                                                              |\n"
		"|                   /$$$$$$                                                    |\n"
		"|                  /$$__  $$                                                   |\n"
		"|                 | $$  \\__/  /$$$$$$  /$$$$$$/$$$   /$$$$$$                   |\n"
		"|                 | $$ /$$$$ |____  $$| $$_  $$_  $$ /$$__  $$                 |\n"
		"|                 | $$|_  $$  /$$$$$$$| $$ \\ $$ \\ $$| $$$$$$$$                 |\n"
		"|                 | $$  \\ $$ /$$__  $$| $$ | $$ | $$| $$_____/                 |\n"
		"|                 |  $$$$$$/|  $$$$$$$| $$ | $$ | $$|  $$$$$$$                 |\n"
		"|                  \\______/  \\_______/|__/ |__/ |__/ \\_______/                 |\n"
		"|                                                                              |\n"
		"|                   /$$$$$$                                                    |\n"
		"|                  /$$__  $$                                 /$$               |\n"
		"|                 | $$  \\ $$ /$$    /$$ /$$$$$$   /$$$$$$   | $$               |\n"
		"|                 | $$  | $$|  $$  /$$//$$__  $$ /$$__  $$  | $$               |\n"
		"|                 | $$  | $$ \\  $$/$$/ | $$$$$$$$| $$  \\__/ |__/               |\n"
		"|                 | $$  | $$  \\  $$$/  | $$_____/| $$                          |\n"
		"|                 |  $$$$$$/   \\  $/   |  $$$$$$$| $$       /$$                |\n"
		"|                  \\______/     \\_/     \\_______/|__/      |__/                |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"+------------------------------------------------------------------------------+";

	std::string gameLogoPage = // was generated by: https://www.asciiart.eu/text-to-ascii-art
		"+------------------------------------------------------------------------------+\n"
		"|            /$$$$$$$                      /$$                                 |\n"
		"|           | $$__  $$                    | $$                                 |\n"
		"|           | $$  \\ $$  /$$$$$$  /$$$$$$$ | $$   /$$  /$$$$$$  /$$   /$$       |\n"
		"|           | $$  | $$ /$$__  $$| $$__  $$| $$  /$$/ /$$__  $$| $$  | $$       |\n"
		"|           | $$  | $$| $$  \\ $$| $$  \\ $$| $$$$$$/ | $$$$$$$$| $$  | $$       |\n"
		"|           | $$  | $$| $$  | $$| $$  | $$| $$_  $$ | $$_____/| $$  | $$       |\n"
		"|           | $$$$$$$/|  $$$$$$/| $$  | $$| $$ \\  $$|  $$$$$$$|  $$$$$$$       |\n"
		"|           |_______/  \\______/ |__/  |__/|__/  \\__/ \\_______/ \\____  $$       |\n"
		"|                                                              /$$  | $$       |\n"
		"|                                                             |  $$$$$$/       |\n"
		"|                                                              \\______/        |\n"
		"|            /$$   /$$                                                         |\n"
		"|           | $$  /$$/                                                         |\n"
		"|           | $$ /$$/   /$$$$$$  /$$$$$$$   /$$$$$$                            |\n"
		"|           | $$$$$/   /$$__  $$| $$__  $$ /$$__  $$                           |\n"
		"|           | $$  $$  | $$  \\ $$| $$  \\ $$| $$  \\ $$                           |\n"
		"|           | $$\\  $$ | $$  | $$| $$  | $$| $$  | $$                           |\n"
		"|           | $$ \\  $$|  $$$$$$/| $$  | $$|  $$$$$$$                           |\n"
		"|           |__/  \\__/ \\______/ |__/  |__/ \\____  $$                           |\n"
		"|                                          /$$  \\ $$                           |\n"
		"|                                         |  $$$$$$/                           |\n"
		"|                                          \\______/                            |\n"
		"+------------------------------------------------------------------------------+";

	// Switch the colors status of the Main Menu page
	void updateMainMenuColorsStatus() {
		const char* colorStatus = isColorsEnabled ? "Enabled!" : "Disabled";
		mainMenuPage =
			"+------------------------------------------------------------------------------+\n"
			"|                                                                              |\n"
			"|                               Main Menu                                      |\n"
			"|                                                                              |\n"
			"|  (1) Start a new game                                                        |\n"
			"|  (2) Enable screen colors (" + std::string(colorStatus) + ")                                         |\n"
			"|  (8) Present instructions and keys                                           |\n"
			"|  (9) EXIT                                                                    |\n"
			"|  >>                                                                          |\n"
			"|                                                                              |\n"
			"+------------------------------------------------------------------------------+";
	}
	
	std::string emptyFrame =
		"+------------------------------------------------------------------------------+\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"|                                                                              |\n"
		"+------------------------------------------------------------------------------+";

public:
	
	Menu(Screen& screen) : screen(screen), _userChoice('\0') {}  // arbitrary temp value for _userChoice

	// Set the user's menu choice based on input
	void setUserChoice() { // was take from perplexity: "how to validate one char inpute from a user in c++"
		std::string input;
		std::getline(std::cin, input);	
		_userChoice = (input.length() == MAX_INPUT_SIZE) ? input[0] : BAD_INPUT_CHAR;	
	}
	// Get the user's current menu choice
	char getUserChoice() { return _userChoice; }

	//Get fom the user int 
	void setUserChoiceInt();

	//Get fom the user choice 
	int getUserChoiceInt() { return _userChoiceInt; }

	std::vector<std::string> getListOfLevels() const  { return filesNamesSorted; }

	// Enable the colors in the game screen
	void switchColorsStatus() { 
		isColorsEnabled = !isColorsEnabled;
		screen.switchColorsStatus();
	}

	// Get if the colors of the screen are enabled
	bool getIsColorsEnabled() { return isColorsEnabled; }

	// Get the total number of instruction pages
	int getNumOfInstructionsPages() const { return NUM_OF_INSTRUCTIONS_PAGES; }

	// Retrieve a specific instruction page by its index
	std::string getInstructionsPage(int pageIndex) const { return instructions[pageIndex]; }

	// Print the game logo page, typically displayed at the start of the game
	void printGameLogoPage() const;

	// Print the main menu page, allowing the user to select options (e.g., Start Game, Instructions, Exit)
	void printMainMenuPage() const ;

	// Print the goodbye page, displayed when the player exits the game
	void printGoodByePage() const;

	// Print a page indicating invalid input, prompting the user to retry
	void printInvalidInputPage() const;

	// Print the pause game page, displayed when the player pauses the game
	void printPauseGamePage() const;

	// Print the game-over page, displayed when the player loses the game
	void printGameOverPage() const;

	// Print the win game page, displayed when the player wins the game
	void printWinGamePage() const;

	// Print the next level page, displayed when the player advances to the next level.	
	void printNextLevelPage() const;

	// Start the main menu loop, handling user input and menu navigation
	bool startMainMenu();

	// Print a results string in a frame to the screen
	void printResultsPage(std::string results) const;

	// Display the instructions menu, including all instructions pages
	void startInstructions();

	//Let the user to select board according to the limitations
	bool selectGameBoard();

	//Return wich board selected 
	std::string getSelectedBoard() const { return selectedBoard; }

	//Extract all the numbers or strings that between the donk_ to .screen 
	std::vector<std::string> extractScreenFilesNumbers();

	//Print all the files name 
	void printAllFiles(std::vector<std::string> fileNumbers) const;

	// Make sleep time between pages of the game
	void sleepBetweenPages() const { Sleep(TIME_BETWEEN_PAGES); }
};
