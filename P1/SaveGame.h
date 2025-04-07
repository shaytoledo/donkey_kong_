#pragma once
#include <fstream>
#include"Results.h"
#include "steps.h"
#include "Game.h"

class SaveGame : public Game {
private:

	Steps currSteps;
	Results currResults;

public:
	SaveGame() : Game() {};
	
	// The function clean the steps and results files for each new game.	
	void initResources() override;

	// The function retrieve the Steps object.
	Steps& getCurrSteps() { return currSteps; }

	// The function retrieve the Results object.
	Results& getCurrResults() { return currResults; }

	// The function set the random seed for the game and save it to the steps.
	void startRandomSeed() override { 
		auto seed = static_cast<long>(std::chrono::system_clock::now().time_since_epoch().count());
		getCurrSteps().setRandomSeed(seed);
		srand(seed);
	}
	
	// The function retrieve from keyboard the next action for mario and save it to the steps. 
	void retrieveMarioAction(Mario& mario, Time& timer) override;

	// The function finish the game when mario lose. It save the steps and results to the files.
	void loseGame() {
		getCurrResults().addResult(getCurrIteration(), Results::ResultValue::lose, getScore());
		Game::loseGame();
		std::string resultFileName = changeExtension(getBoard().getCurrLevel(), getResultExt());
		std::string stepsFileName = changeExtension(getBoard().getCurrLevel(), getStepExt());
		getCurrSteps().saveSteps(stepsFileName);
		getCurrResults().saveResults(resultFileName);
		getCurrResults().init();
		getCurrSteps().init();
	}

	// The function finish a level in the game. It save the steps and results to the files.
	void finishLevel(Mario& mario, BarrelsManager& BManager, GhostsManager& GManager, Time& timer) override;

	// The function kill Mario and save the result to the results.
	void killMario(Mario& mario, bool isByEnemy) override {
		Game::killMario(mario, isByEnemy);
		Results::ResultValue result = isByEnemy ? Results::ResultValue::hitEnemy : Results::ResultValue::fellToHigh;
		getCurrResults().addResult(getCurrIteration(), result, getScore());
	}
};

