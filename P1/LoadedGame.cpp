#include "LoadedGame.h"
#include <ios>
#include <iostream>

// Load the resources from the files (i.e steps and results files)
void LoadedGame::loadResources(std::vector <std::string> stepsFilesNames, std::vector <std::string> resultsFileNames) {
	for (auto& fileName : stepsFilesNames) {
		Steps newObj = Steps::load(fileName);
		newObj.setFileName(fileName);
		allSteps.push_back(newObj);
	}
	for (auto& fileName : resultsFileNames) {
		Results newObj = Results::load(fileName);
		newObj.setFileName(fileName);
		newObj.prepareForPrinting();
		allResults.push_back(newObj);
	}
}

// Start the game
void LoadedGame::start() { 
	
	while (getCurrSteps().isEmpty()) {
		saveIndex++;
	}
	std::string startBoardFileName = changeExtension(getCurrSteps().getFileName(), getScreenExt());
	setStartBoard(startBoardFileName);
	startPlay();
}

// Retrieve mario action accourding to the steps file and the current iteration number
void LoadedGame::retrieveMarioAction(Mario& mario, Time& timer) {
	(void) timer;
	for (int i = 0; i < getMaxNumOfUserInputs(); i++) {
		size_t saved_iter = getCurrSteps().getCurrIteration();
		if (saved_iter == getCurrIteration()) {
			char key = getCurrSteps().popStep();
			mario.keyPressed(key);
		}
		Sleep(getIntervalTime());
	}
}
// Finish the level and start the next level if needed
void LoadedGame::finishLevel(Mario& mario, BarrelsManager& BManager, GhostsManager& GManager, Time& timer) {
	updateLevelCompletionScore(getCurrIteration()); // so mario will have its final score before checking the results
	checkResults(Results::ResultValue::finished);
	printResults();

	if (saveIndex + 1 != allSteps.size()) {
		saveIndex++;
		std::string _currBoardFileName = changeExtension(getCurrSteps().getFileName(), getScreenExt());
		getBoard().setCurrLevel(_currBoardFileName);
		(void)getBoard().setBoard();
		makeNextLevelObjects(mario, BManager, GManager, timer);
		startRandomSeed();
		initIteration();
	}
	else {
		winGame();
	}
}
// Check if the results are valid (i.e the results file match the steps file)
void LoadedGame::checkResults(Results::ResultValue resValue) {
	auto result = getCurrResults().popResult();
	bool is_incorrect_iter = std::get<0>(result) != getCurrIteration();
	bool is_incorrect_result_code = std::get<1>(result) != resValue;
	bool is_incorrect_score = std::get<2>(result) != getScore();

	if (is_incorrect_iter || is_incorrect_result_code || is_incorrect_score) {
		throw std::runtime_error("The result file dosn't match to the steps file.");
	}
}

void LoadedGame::loseGame() {
	checkResults(Results::ResultValue::lose);
	printResults();
	Game::loseGame();
}

void LoadedGame::winGame() {
	if (getIsSilentMode()) {
		setIsPlaying(false);
	}
	else {
		Game::winGame();
	}
}

void LoadedGame::printResults() {
	getScreen().setSilentMode(false);
	getMenu().printResultsPage(getCurrResults().getResultsForPrinting());
	getScreen().setSilentMode(getIsSilentMode());
}

std::vector<std::string> LoadedGame::getlevelsFilesNames() const {
	std::vector <std::string> loadedFilesName;
	std::vector <std::string> screenFilesName = findFiles(getResourceFilePre(), getScreenExt());
	std::vector <std::string> stepsFilesName = findFiles(getResourceFilePre(), getStepExt());
	for (auto& fileName : stepsFilesName) {
		auto it = std::find(screenFilesName.begin(), screenFilesName.end(), fileName);
		loadedFilesName.push_back(*it);
	}
	return loadedFilesName;
}