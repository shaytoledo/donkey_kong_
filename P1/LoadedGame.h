#pragma once
#include <stdexcept>
#include <string.h>
#include"Results.h"
#include "steps.h"
#include "Game.h"
#include "Utils.h"

class LoadedGame : public Game {
private:
	static constexpr float MOVE_INTERVAL_TIME = 12.5, SPEED_MULTIPLIER = 2, SILENT_MOVE_INTERVAL_TIME = 0.001, SILENT_SPEED_MULTIPLIER = 25000;
	std::string currBoardFileName;
	std::vector <Steps> allSteps;
	std::vector <Results> allResults;
	int saveIndex = 0;

public:
	LoadedGame(bool isSilentMode) : Game(isSilentMode)
	{
		auto stepsFileNames = findFiles(getResourceFilePre(), getStepExt());
		auto resultsFileNames = findFiles(getResourceFilePre(), getResultExt());
		loadResources(stepsFileNames, resultsFileNames);
	};
	
	// Getters
	float getIntervalTime() const override{ return getIsSilentMode() ? SILENT_MOVE_INTERVAL_TIME : MOVE_INTERVAL_TIME; }
	float getSpeedMultiplier() const override { return getIsSilentMode() ? SILENT_SPEED_MULTIPLIER : SPEED_MULTIPLIER; }
	Steps& getCurrSteps() { return allSteps.at(saveIndex); }
	Results& getCurrResults() { return allResults.at(saveIndex); }
	std::string getStartBoard() const override { return currBoardFileName; }
	std::vector<std::string> getlevelsFilesNames() const override;

	// Setters
	void setStartBoard(std::string fileName) { currBoardFileName = fileName; }
	
	// Start the Game
	void start() override;
	
	// Handle the case where the board is not loaded
	void handleUnloadedBoards() override {
		throw std::runtime_error("Unable to load Board: " + getBoard().getCurrLevel());
		return;
	}
	// Load the resources (i.e Steps files and Results files)
	void loadResources(std::vector <std::string> stepsFilesNames, std::vector <std::string> resultsFileNames);

	// Get the random seed for the current level from the steps file and set it for the game
	void startRandomSeed() override { srand(getCurrSteps().getRandomSeed()); }
	
	// Retrieve mario action accourding to the steps file and the current iteration number
	void retrieveMarioAction(Mario& mario, Time& timer) override;
	
	// Decrease mario lives acourding to the Results file
	void killMario(Mario& mario, bool isByEnemy) override {
		auto result = getCurrResults().popResult();
		if (getCurrIteration() != std::get<0>(result) || ((!isByEnemy || std::get<1>(result) != Results::ResultValue::hitEnemy) && std::get<1>(result) != Results::ResultValue::fellToHigh)) {
			throw std::runtime_error("Result file dont match to the steps file.");
		}
		Game::killMario(mario, isByEnemy);
	}

	// Finish the current level and start the next level if needed
	void finishLevel(Mario& mario, BarrelsManager& BManager, GhostsManager& GManager, Time& timer) override;
	
	// Finish the game by losing
	void loseGame() override;
	
	// Finish the game by wining
	void winGame() override;

	// Print the results of the current level to the screen
	void printResults();

	// Check if the results are valid (i.e the results file match the steps file)
	void checkResults(Results::ResultValue resValue);
	
};

