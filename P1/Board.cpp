#include "Board.h" 


// Reset the board to its original state
void Board::reset() {  // was taken from lab excercise
	for (int i = MIN_COORD; i < MAX_Y; i++) {
		memcpy(currentBoard[i], originalBoard[i], MAX_X + 1);
		currentBoard[i][MAX_X] = '\n';
	}
}

// Print the board to the console
void Board::print() const {  // was taken from lab excercise
	bool resetColor = false;

	for (int i = MIN_COORD; i < MAX_Y - 1 ; i++) {

		//For printing with coclors in str 	
	    std::string str(currentBoard[i], 81);
		screen.coloredStrPrint(str, DEFAULT_COLOR, resetColor);
	}
	std::string str(currentBoard[MAX_Y - 1], 80);
	str = str + '\0';
	screen.coloredStrPrint(str, DEFAULT_COLOR, resetColor);
}

// Check if a given coordinate is within valid boundaries
bool Board::isValidCoord(Coord pos) const {
	return (!(pos.x >= MAX_X  || pos.x <= MIN_COORD || pos.y >= MAX_Y - 1 || pos.y <= MIN_COORD) && (!isFloor(pos)) && (!isEdge(pos)));
}

// Check if a given position is a floor
bool Board::isFloor(Coord pos) const {
	switch (getChar(pos)) {
	case blocksChars[0]: // '=' 
	case blocksChars[1]: // '<'
	case blocksChars[2]: // '>'
		return true;
	default:
		return false;
	}
}

// Print Mario's remaining lives to the screen
void Board::printLivesAndScore(int lives, int score) const {
	gotoxy(liveAndScorePos);
	std::string toPrint = "LIVES: " + std::to_string(lives) + "  SCORE: " + std::to_string(score);
	screen.coloredStrPrint(toPrint);
}

// Function to set up the game board based on a file and initial conditions
bool Board::setBoard(const bool flag) {

	// Clear the starting positions of ghosts
	ghostsStartPos.clear();
	barrelsStartPos.clear();
	const std::string fileName = currLevel ;

	// Check if the board is valid and if the function was called from the menu or for the next level
	if (!isValidBoard(fileName, flag)) { return false; }

	std::ifstream screen_file(fileName);
	int curr_row = 0, curr_col = 0;
	char c;

	// Read the file character by character until EOF or the maximum row limit is reached
	while (!screen_file.get(c).eof() && curr_row < MAX_Y) {

		if (c == '\n') {
			if (curr_col < MAX_X) {
				// add spaces for missing cols
				fill(originalBoard[curr_row] + curr_col, MAX_X - curr_col, ' ', curr_row);
			}
			++curr_row;
			curr_col = 0;
			continue;
		}
		// Process characters and update the board if within column limits
		if (curr_col < MAX_X) {
			processSpecialCharacters(c, curr_col, curr_row);
			originalBoard[curr_row][curr_col] = c;
			curr_col++;
		}
		originalBoard[curr_row][MAX_X] = '\n';
	}

	if (curr_col < MAX_X) {	// add spaces for missing cols in the last row

		fill(originalBoard[curr_row] + curr_col, MAX_X - curr_col, ' ', curr_row);
		curr_row++;
	}

	// Fill the remaining rows with spaces if the board is incomplete
	while (curr_row < MAX_Y - 1) {	// add spaces for missing for all missing rows

		fill(originalBoard[curr_row], MAX_X, ' ', curr_row);
		curr_row++;
	}

	// Add the frame around the board
	addFrame();

	// Place special elements on the board
	originalBoard[paulinaPos.y][paulinaPos.x] = '$';
	if (barrelsStartPos.size() > 0) { originalBoard[barrelsStartPos.at(0).y][barrelsStartPos.at(0).x] = '&'; } // meanwhile, we implement only one donkeykong
	Coord defultHammerPos = { -1,-1 };
	if (getHammerPos() != defultHammerPos) {
		originalBoard[hammerPos.y][hammerPos.x] = 'p';

	}
		screen_file.close();
		return true;
	
}

//Check if the board is valid 
bool Board::isValidBoard(const std::string fileName, bool flag) const {
	std::ifstream screen_file(fileName);
	
	if (screen_file.is_open() && checkCurrBoard(screen_file, flag)) {
		screen_file.close();
		return true;
	}
	else {
		screen_file.close();
		return false;
	}
}

//Check if the file is good and if the board in the file valid 
bool Board::checkCurrBoard(std::ifstream& screen_file,bool flag) const
{
	int paulinaCounter = 0, marioCounter = 0, kongCounter = 0, liveAndScoreCounter = 0, curr_row = 0, curr_col = 0;
	char c;

	while (!screen_file.get(c).eof() && curr_row < MAX_Y) {

		if (c == '\n') {
			++curr_row;
			curr_col = 0;
			continue;
		}

		if (curr_col < MAX_X) {
			if (c == '@') { marioCounter++;}
			else if (c == '$') { paulinaCounter++; }
			else if (c == '&') { kongCounter++; }
			else if (c == 'L') { liveAndScoreCounter++; }
			curr_col++;
		}
	}
	screen_file.close();
	if (flag) { checkAndReportMissingElements(marioCounter, paulinaCounter, liveAndScoreCounter, kongCounter); }
	return (marioCounter != 0 && paulinaCounter != 0 && liveAndScoreCounter != 0&& kongCounter != 0);
}

//Report about which Elements are missing
void Board::checkAndReportMissingElements(int& marioCounter, int& paulinaCounter, int& liveAndScoreCounter, int& kongCounter) const {

	if (marioCounter == 0) { std::cout << "There isn't mario on board" << std::endl; }
	if (paulinaCounter == 0) { std::cout << "There isn't paulina on board" << std::endl; }
	if (liveAndScoreCounter == 0) { std::cout << "There isn't live and score on board" << std::endl; }
	if (kongCounter == 0) { std::cout << "There isn't donkey kong on board" << std::endl; }

	if (marioCounter == 0 || paulinaCounter == 0 || liveAndScoreCounter == 0 || kongCounter == 0) {
		std::cout << "Press a key to continue." << std::endl;
		(void)_getch();
		screen.clear();
	}
}

//Initializes the positions of objects on the board.
void Board::processSpecialCharacters(char& c, int curr_col,int curr_row)  {
	//handle special chars
	switch (c) {
	case '@':
		c = ' ';
		marioStartPos = { curr_col, curr_row };
		break;

	case '$':
		c = ' ';
		paulinaPos = { curr_col, curr_row };
		break;

	case '&':
		c = ' ';
		barrelsStartPos.push_back({curr_col, curr_row});
		break;

	case 'x':
		c = ' ';
		ghostsStartPos.push_back({ curr_col, curr_row });
		break;
	
	case 'X':
		c = ' ';
		ghostsStartPos.push_back({ curr_col, curr_row });
		superGhostsStartPos.push_back({ curr_col, curr_row });
		break;

	case 'p':
		c = ' ';
		hammerPos = { curr_col, curr_row };
		break;

	case 'L':
		c = ' ';
		liveAndScorePos = { curr_col, curr_row };
		break;

	default:
		if (c != '=' && c != '<' && c != '>' && c != 'Q' && c != 'H') {
			c = ' ';
		}
		break;
	}
}

// Add frame to the board (Q)
void Board::addFrame()  {

	// add a closing frame
	// first line
	#pragma warning(suppress : 4996) // to allow strcpy
	strcpy(originalBoard[0], std::string(MAX_X, 'Q').c_str());
	originalBoard[0][MAX_X] = '\n';
	// last line
	#pragma warning(suppress : 4996) // to allow strcpy
	strcpy(originalBoard[MAX_Y - 1], std::string(MAX_X, 'Q').c_str());//The last row from the file will be the last row in the board 
	originalBoard[0][MAX_X] = '\0';

	// first col + last col
	for (int row = 1; row < MAX_Y - 1; ++row) {//MAX_Y - 1 was curr_row
		originalBoard[row][0] = 'Q';
		originalBoard[row][MAX_X - 1] = 'Q';
	}
}

// Fill the line with some value and add \n
void Board::fill(char* dest,int count,char val, int curr_row) {
	std::fill_n(dest, count, val);
	originalBoard[curr_row][MAX_X] = '\n';
}
