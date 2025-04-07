#pragma once

#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <conio.h>
#include <string>
#include <algorithm> 
#include "Menu.h"
#include "Board.h"
#include "utils.h"
#include "Mario.h"
#include "Screen.h"
#include "GhostsManager.h"
#include "BarrelsManager.h"
#include "Time.h"

class Game {
private:
	
	static constexpr int RESTART_BOARD_TIME = 500, ESC = 27, BEST_TIME_SCORE = 500, WORST_TIME_SCORE = 750, MAX_NUM_OF_USER_INPUTS = 2;
	static constexpr float MOVE_INTERVAL_TIME = 25, SPEED_MULTIPLIER = 1;
	bool isPlaying = false;
	int score = 0;
	static constexpr const char* ResourceFilePrefix = "dkong_";
	static constexpr const char* stepsExt = ".steps";
	static constexpr const char* resultExt = ".result";
	static constexpr const char* screenExt = ".screen";

	Screen screen;
	Board board;

	// Menu
	Menu menu;
	
	enum class Points {
		aboveGhost = 20,
		aboveBarrel = 10,
		nextLevel = 100,
		killGhost = 40,
		kiilBarrel = 30
	};
	int iteration = 0;
	bool isSilentMode;

public:
	Game(bool _isSilentMode = false) : isSilentMode(_isSilentMode), screen(_isSilentMode), menu(screen), board(screen, "") {}

	// Getters and setters
	std::string getStepExt() const { return std::string(stepsExt); }
	std::string getResultExt() const { return std::string(resultExt); }
	std::string getResourceFilePre() const { return std::string(ResourceFilePrefix); }
	std::string getScreenExt() const { return std::string(screenExt); }
	virtual std::string getStartBoard() const { return menu.getSelectedBoard();} 
	virtual std::vector<std::string> getlevelsFilesNames() const { return menu.getListOfLevels(); }
	int getMaxNumOfUserInputs() const { return MAX_NUM_OF_USER_INPUTS;  }
	void setCurrentLevelFileName(std::string fileName) {}
	bool getIsPlaying() const { return isPlaying; }
	void setIsPlaying(bool _isPlaying) { this->isPlaying = _isPlaying; }
	virtual float getIntervalTime() const { return MOVE_INTERVAL_TIME; }
	virtual float getSpeedMultiplier() const { return SPEED_MULTIPLIER; }
	int getEscCode() const { return ESC; }
	int getCurrIteration() const { return iteration; }
	void incrementIteration() { iteration++; }
	void initIteration(int num = -1) { iteration = num; }
	int getScore() const { return score; }
	void initScore() { score = 0; }
	int incrementScore(int points) { return score += points; }
	bool getIsSilentMode() const { return isSilentMode; }
	Screen& getScreen() { return screen; }
	Board& getBoard() { return board; }
	Menu& getMenu() { return menu; }

	// Start the game 
	virtual void start();

	// Start game loop (and menge the game)
	void startPlay();

	// Puse the game for break
	void pauseGame(Time& timer);

	void makeNextLevelObjects(Mario& mario, BarrelsManager& BManager, GhostsManager& GManager, Time& timer);

	// Reset the game 
	void initLevelObjects(Mario& mario, BarrelsManager& BManager, GhostsManager& GManager);

	// Func for game over screen 
	virtual void loseGame();

	// Print the life of mario in the game 
	void printLivesAndScore(int live) { getBoard().printLivesAndScore(live, getScore()); }

	// Winning func 
	virtual void winGame();
	
	// finish the level and start the next level if needed
	virtual void finishLevel(Mario& mario, BarrelsManager& BManager, GhostsManager& GManager, Time& timer);

	//Update the score in each loop of the game 
	void updateMidLevelScore(Mario& mario, BarrelsManager& BManager, GhostsManager& GManager);

	//Update the level after winning in curr level
	void updateLevelCompletionScore(int time);

	// Handle the objects damage (Ghosts and Barrels)
	void handleObjectsDamage(Mario& mario, BarrelsManager& BManager, GhostsManager& GManager);

	//Check if there is any objects under mario 
	template <typename T>
	bool isObjectUnderMario(GameObjectsManager<T>& manager, Coord currPos);

	// Handle the unloaded boards
	virtual void handleUnloadedBoards() {
		menu.selectGameBoard();
		getBoard().setCurrLevel(menu.getSelectedBoard());
	}

	// Get next mario action.
	virtual void retrieveMarioAction(Mario& mario, Time& timer);
	
	// Set the random seed for the game
	virtual void startRandomSeed() { srand(static_cast<long>(std::chrono::system_clock::now().time_since_epoch().count())); }

	// Decrease mario lives
	virtual void killMario(Mario& _mario, bool isByEnemy = false) {
		(void*)isByEnemy;
		_mario.decreaseLives();
	}

	// Init the resources for the game
	virtual void initResources() {}
};