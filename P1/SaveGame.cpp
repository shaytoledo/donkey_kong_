#include "SaveGame.h"

// The function clean the steps and results files for each new game.	
void SaveGame::initResources() {
	initIteration(0);
	initScore();
	auto stepsFileNames = findFiles(getResourceFilePre(), getStepExt());
	auto resultFileNames = findFiles(getResourceFilePre(), getResultExt());

	for (auto& fileName : stepsFileNames) {
		std::ofstream file(fileName, std::ios::out | std::ios::trunc);
	}
	for (auto& fileName : resultFileNames) {
		std::ofstream file(fileName, std::ios::out | std::ios::trunc);
	}
}

void SaveGame::retrieveMarioAction(Mario& mario, Time& timer) {
	for (int i = 0; i < getMaxNumOfUserInputs(); i++) {
		if (_kbhit()) {
			char key = static_cast<char>(_getch());
			if (key == getEscCode()) { pauseGame(timer); }
			mario.keyPressed(key);
			getCurrSteps().addStep(getCurrIteration(), key);
		}
		Sleep(getIntervalTime());
	}
}
void SaveGame::finishLevel(Mario& mario, BarrelsManager& BManager, GhostsManager& GManager, Time& timer) {
	std::string stepsFileName = changeExtension(getBoard().getCurrLevel(), getStepExt());
	std::string resultFileName = changeExtension(getBoard().getCurrLevel(), getResultExt());

	getCurrSteps().saveSteps(stepsFileName);
	Game::finishLevel(mario, BManager, GManager, timer);
	getCurrResults().addResult(getCurrIteration(), Results::ResultValue::finished, getScore());
	getCurrResults().saveResults(resultFileName);
	getCurrSteps().init();
	getCurrResults().init();
	initIteration();
	startRandomSeed();
	std::cout << std::flush;
}