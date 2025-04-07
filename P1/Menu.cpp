#include "Menu.h"

// Display the instructions menu, including all instructions pages
void Menu::startInstructions() {
	int i = 0;
	
	while (i >= 0 && i < NUM_OF_INSTRUCTIONS_PAGES) {
		screen.coloredStrPrint(getInstructionsPage(i), DEFAULT_COLOR);
		gotoxy({ 6,13 });
		setUserChoice();

		switch (getUserChoice()) {
		case 'B': case 'b':
			i--;
			break;

		case 'N': case 'n':
			i++;
			break;

		case 'M': case 'm': // end the function to return to the switch case of startMainMenu()
		
			break;

		default:
			printInvalidInputPage();
			break;
		}
		screen.clear();
	}
}

// Start the main menu loop, handling user input and menu navigation
bool Menu::startMainMenu() {

	printMainMenuPage();
	setUserChoice();
	while (getUserChoice() != '1' && getUserChoice() != '2' && getUserChoice() != '8' && getUserChoice() != '9') {
		printInvalidInputPage();
		printMainMenuPage();
		setUserChoice();
	}

	switch (getUserChoice()) {
	case '1':
		return selectGameBoard();;
		break;

	case '2':
		screen.clear();
		switchColorsStatus();
		updateMainMenuColorsStatus();
		return startMainMenu();
		break;

	case '8':
		screen.clear();
		startInstructions();
		return startMainMenu();
		break;

	case '9':
		printGoodByePage();
		return false;
		break;

	default:
		return false;
	}
}

// Print the game-over page, displayed when the player loses the game
void Menu:: printGameLogoPage() const {
	screen.coloredStrPrint(gameLogoPage, DEFAULT_COLOR);
	Sleep(TIME_OF_START_GAME);
	screen.clear();
}

// Print the main menu page, allowing the user to select options (e.g., Start Game, Instructions, Exit)
void Menu::printMainMenuPage() const {
	screen.coloredStrPrint(mainMenuPage, DEFAULT_COLOR);
	gotoxy({ 7, 8 });
}

// Print the goodbye page, displayed when the player exits the game
void Menu::printGoodByePage() const {
	screen.clear();
	screen.coloredStrPrint(goodByePage, DEFAULT_COLOR);
	sleepBetweenPages();
	screen.clear();
}

void Menu::printNextLevelPage() const {
	screen.clear();
	screen.coloredStrPrint(nextLevel, DEFAULT_COLOR);
	sleepBetweenPages();
	screen.clear();
}

// Print a page indicating invalid input, prompting the user to retry
void Menu::printInvalidInputPage() const {
	screen.clear();
	screen.coloredStrPrint(invalidInputPage, screen.getColor("invalidInput"));
	sleepBetweenPages();
	screen.clear();
}

// Print the pause game page, displayed when the player pauses the game
void Menu::printPauseGamePage() const {
	screen.clear();
	screen.coloredStrPrint(pauseGamePage, DEFAULT_COLOR); 
}

// Print the win game page, displayed when the player wins the game
void Menu::printWinGamePage() const {
	screen.clear();
	screen.coloredStrPrint(winGamePage, screen.getColor("win"));
}

// Print the game-over page, displayed when the player loses the game
void Menu::printGameOverPage() const {
	screen.clear();
	screen.coloredStrPrint(gameOverPage, screen.getColor("gameOver"));
	sleepBetweenPages();
	screen.clear();
}

// Print a results string with a frame to the screen 
void Menu::printResultsPage(std::string results) const {
	screen.clear();
	screen.coloredStrPrint(emptyFrame, DEFAULT_COLOR);
	gotoxy({ 25, 4});
	screen.coloredStrPrint(results, DEFAULT_COLOR);
	Sleep(3000);
	sleepBetweenPages();
	screen.clear();
}

//Extract all the numbers or strings that between the donk_ to .screen 
std::vector<std::string> Menu::extractScreenFilesNumbers() {

	const std::string folderPath = ".";
	const std::string fileNamePrefix = "dkong_";
	const std::string fileNameExtension = ".screen";

	std::vector<std::string> filesNumbers;

	for (const auto& entry : std::filesystem::directory_iterator(folderPath)) { // iterates over all files
		if (std::filesystem::is_regular_file(entry)) {// Checks if it is a file and not a folder
			std::string filename = entry.path().filename().string();//take the file name 
			// Check if the filename starts with baseFileName and ends with lastFileName
			if (filename.starts_with(fileNamePrefix) && filename.ends_with(fileNameExtension)) {
				filesNumbers.push_back(filename);
			}
		}
	}
	// sort all files 
	std::sort(filesNumbers.begin(), filesNumbers.end());
	return filesNumbers;
};

//Let the user to select board according to the limitations
bool Menu::selectGameBoard() {
	screen.clear();

	// get all the files name 
	filesNamesSorted = extractScreenFilesNumbers();

	//if there isn't goog file 
	if (filesNamesSorted.size() == 0) {
		std::string str = "EMPTY_SCREENS_DIR_ERROR";
		screen.coloredStrPrint(str, DEFAULT_COLOR);
		return false;
	}

	// All need to be printed with color
	std::string str2 = "  Please write the file namber: \n";
	screen.coloredStrPrint(str2, DEFAULT_COLOR);
	printAllFiles(filesNamesSorted);

	// get user choice 
	setUserChoiceInt();
	screen.clear();
	int choice = getUserChoiceInt() - 1 ;
	if (choice > filesNamesSorted.size() || choice < 0) {
		printInvalidInputPage();
		return selectGameBoard();
		
	}
	if (choice == filesNamesSorted.size()) { //if chooce defult 
		choice = 0;
	}

	std::string StrChoice = filesNamesSorted[choice];

	auto it = std::find(filesNamesSorted.begin(), filesNamesSorted.end(), StrChoice); //find the selected level 

	if (it != filesNamesSorted.end()) {// if the string in the vector
		selectedBoard = StrChoice;
		return true;
	}
	else if (--it != filesNamesSorted.end()) {// if default file was chosen
		selectedBoard = "1";
		return true;
	}
	else if (filesNamesSorted.size() < 1 && StrChoice == "1") { // if no board was chosen
		screen.clear();
		printGoodByePage();
		screen.clear();
		return false;
	}
	else { // invalid input 
		printInvalidInputPage();
		return selectGameBoard();
	}

}

//Get from the user the input 
void Menu::setUserChoiceInt() {
	_userChoiceInt = 0;
	std::string input;
	std::getline(std::cin, input);

	for (char c : input) {
		if (std::isdigit(c)) {
			continue;
		}
		else {
			_userChoiceInt = BAD_INPUT_CHAR;
		}
	}

	if (_userChoiceInt != BAD_INPUT_CHAR) {
		_userChoiceInt = std::stoi(input);
	}

}

//Print all the files name 
void Menu::printAllFiles(std::vector<std::string> fileNumbers) const {

	int i;
	for (i = 1; i <= fileNumbers.size(); i++) {
		std::string string_number = std::to_string(i);
		std::string fileNameOut = "  (" + string_number + ") " + fileNumbers[i - 1] + '\n';
		screen.coloredStrPrint(fileNameOut, DEFAULT_COLOR);
	}
	std::string string_number = std::to_string(i);
	std::string defultChooice = "  (" + string_number + ") default \n";
	screen.coloredStrPrint(defultChooice, DEFAULT_COLOR);
	screen.coloredStrPrint("  >>  ", DEFAULT_COLOR);
	}
