#include "utils.h"

void gotoxy(Coord pos) {
    std::cout.flush();
    COORD coord;
    coord.X = static_cast <short int> (pos.x);
    coord.Y = static_cast <short int> (pos.y);
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); // Move the cursor 
}

void ShowConsoleCursor(bool showFlag) { 
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

std::vector <std::string> findFiles(std::string prefix, std::string ext) {

	const std::string folderPath = ".";

	std::vector<std::string> filesNumbers;
	for (const auto& entry : std::filesystem::directory_iterator(folderPath)) { // iterates over all files
		if (std::filesystem::is_regular_file(entry)) {// Checks if it is a file and not a folder
			std::string fileName = entry.path().filename().string();//take the file name 

			// Check if the filename starts with baseFileName and ends with lastFileName
			if (fileName.starts_with(prefix) && fileName.ends_with(ext)) {
				// Extract the exclusive name between baseFileName and lastFileName
				//std::string fileExName = filename.substr(prefix.length(), filename.length() - prefix.length() - ext.length());
				filesNumbers.push_back(fileName);
			}
		}
	}
	// sort all files 
	std::sort(filesNumbers.begin(), filesNumbers.end());
	return filesNumbers;
}

std::string changeExtension(const std::string& filename, const std::string& newExtension) { // was taken from chatGpt:"how to change file extension"
    // Find the last occurrence of '.'
    size_t dotPos = filename.rfind('.');
    if (dotPos == std::string::npos) {
        throw std::invalid_argument("Filename does not contain an extension.");
    }
    // Replace everything after the '.' with the new extension
    return filename.substr(0, dotPos) + newExtension;
}

// Define the static members of the Direction class
const Direction Direction::STAY = { 0, 0 };
const Direction Direction::UP = { 0, -1 };
const Direction Direction::DOWN = { 0, 1 };
const Direction Direction::LEFT = { -1, 0 };
const Direction Direction::RIGHT = { 1, 0 };