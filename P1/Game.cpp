#include "Game.h"

// start the game 
void Game::start() {
	bool toStartGame = true;
	while (toStartGame) {
		menu.printGameLogoPage();
		toStartGame = menu.startMainMenu();
		if (toStartGame) {
			initResources();
			startPlay();
		}
	}
}                                                                                                                                                                                                                                                                                                                                                                                                                                                      

// start game loop (game logic here)
void Game::startPlay() {
	setIsPlaying(true);
	screen.clear();
	ShowConsoleCursor(false);
	getBoard().setCurrLevel(getStartBoard());
	while (!getBoard().setBoard(true)) {
		handleUnloadedBoards();
	}
	getBoard().reset();
	
	Mario mario(getBoard(), screen, getBoard().getMarioStartPos());
	BarrelsManager BManager(getBoard(), screen);
	GhostsManager GManager(getBoard(), screen, getBoard().getGhostsStartPos(), getBoard().getSuperGhostsStartPos());
	BManager.addObject();

	
	Time timer(getSpeedMultiplier(), screen);
	startRandomSeed();

	//reset
	board.print();

	while (getIsPlaying()) { 
		printLivesAndScore(mario.getLives());
		timer.showElapsedTime();

		// draw objects
		mario.draw();
		mario.setUseingHammer(false);
		BManager.addObject();
		GManager.addAllObjects(mario);
		BManager.drawObjects();
		GManager.drawObjects();
		
		// keyboard - player interaction
		retrieveMarioAction(mario, timer);

		BManager.moveObjects();
		GManager.moveObjects();

		mario.erase();
		handleObjectsDamage(mario, BManager, GManager);
		mario.move();
		handleObjectsDamage(mario, BManager, GManager);

		if (mario.wasFellTooHigh()) {
			mario.getDown();
			killMario(mario);
			initLevelObjects(mario, BManager, GManager);	
		}
		
		updateMidLevelScore(mario, BManager, GManager);
		
		if (mario.isDead()) {
			loseGame();
		}

		if (board.isMarioReachPaulina(mario.getPos())) {
			finishLevel(mario, BManager, GManager, timer);
		}
		if (!mario.isHoldingHammer() && (board.getChar(mario.getPos()) == mario.getHammerSymbol())) {
			mario.setHoldingHammer(true);
			board.eraseHammer();
		}
		incrementIteration();
	}
}

// finish the current level and start the next level of needed
void Game::finishLevel(Mario& mario, BarrelsManager& BManager,GhostsManager& GManager, Time& timer) {
	updateLevelCompletionScore(getCurrIteration());
	std::vector<std::string> listOfLevels = getlevelsFilesNames();
	std::string currLevel = board.getCurrLevel();
	auto it = std::find(listOfLevels.begin(), listOfLevels.end(), currLevel);
	it++;

	if (it != listOfLevels.end()) {
		board.setCurrLevel(*it);
		while (!board.setBoard()) {
			it++;
			if (it != listOfLevels.end()) {
				board.setCurrLevel(*it);
			}
			else {
				winGame();
				return;
			}
		}
		makeNextLevelObjects(mario, BManager, GManager, timer);
	}
	else {
		winGame();
	}
}

// pause the game for a break
void Game::pauseGame(Time& timer ) { 
	timer.pause();
	menu.printPauseGamePage();
	while (true) {
		if (_kbhit()) {
			char choose = static_cast<char>(_getch());
			if (choose == ESC) {
				screen.clear();
				Sleep(MOVE_INTERVAL_TIME);
				board.print();
				timer.resume();
				return;
			}
		}
	}
}

void Game::makeNextLevelObjects(Mario& mario, BarrelsManager& BManager, GhostsManager& GManager, Time& timer) {
	board.reset();
	mario.setStartPos(board.getMarioStartPos());
	mario.setHoldingHammer(false);
	BManager.setStartPos(board.getBarrelsStartPos().at(0)); // meanwhile, take the first donkeykong positon
	GManager.setStartPositions(board.getGhostsStartPos());
	GManager.setMaxNumOfObjects(static_cast<int>(board.getGhostsStartPos().size()));
	menu.printNextLevelPage();
	board.print();
	initLevelObjects(mario, BManager, GManager);
	timer.reset();
}

// reset the level objects
void Game::initLevelObjects(Mario& mario, BarrelsManager& BManager, GhostsManager& GManager) {
	BManager.resetAllObjects();
	
	GManager.resetAllObjects();
	GManager.setWereAllObjectsAdded(false);
	
	mario.sendToStart();
	mario.initFallCounter();

	if (mario.isHoldingHammer()) {
		getBoard().printHammer();
		mario.setHoldingHammer(false);
	}

	Sleep(RESTART_BOARD_TIME * (1/getSpeedMultiplier()));
	BManager.addObject();
}

// print game over screen and start the game again
void Game::loseGame() {
	menu.printGameOverPage();
	setIsPlaying(false);
}

// print the wining game screen and start the game again 
void Game::winGame() {
	menu.printWinGamePage();
	int ch = _getch();
	(void)ch;
	getMenu().sleepBetweenPages();
	screen.clear();
	setIsPlaying(false);
}

// update the score of the player in the middle of the level
void Game::updateMidLevelScore(Mario& mario, BarrelsManager& BManager, GhostsManager& GManager) {

	if (isObjectUnderMario(GManager, mario.getPos())) {
		incrementScore(static_cast<int>(Points::aboveGhost));
	}

	if (isObjectUnderMario(BManager, mario.getPos())) {
		incrementScore(static_cast<int>(Points::aboveBarrel));
	}
}

// update the score of the player in the end of the level
void Game::updateLevelCompletionScore(int time) {
	if (time <= BEST_TIME_SCORE) {
		incrementScore(static_cast<int>(Points::nextLevel));
	}

	else if (time <= WORST_TIME_SCORE) {
		incrementScore(static_cast<int>(Points::nextLevel) / 2);
	}

	else {
		incrementScore(static_cast<int>(Points::nextLevel) / 5);
	}

}

// Handle all scenerios of objects damage and collision
void Game::handleObjectsDamage(Mario& mario, BarrelsManager& BManager, GhostsManager& GManager) {
	
	if (BManager.handleBarrelDemaged(mario) || GManager.handleGhostDemaged(mario)) {
		killMario(mario, true);
		initLevelObjects(mario, BManager, GManager);
	}
}

//Check if there is any objects under mario 
template <typename T>
bool Game::isObjectUnderMario(GameObjectsManager<T>& manager, Coord currPos) {
	auto objects = manager.getObjects();
	for (const auto& obj : objects) {
		Coord marioPosMinusOne = currPos;

		marioPosMinusOne.y += 1;
		if (obj->getPos() == marioPosMinusOne ||
			obj->getPos() == Coord(marioPosMinusOne.x + 1, marioPosMinusOne.y) ||
			obj->getPos() == Coord(marioPosMinusOne.x - 1, marioPosMinusOne.y)) {
			return true;
		}
	}
	return false;
}


void Game::retrieveMarioAction(Mario& mario, Time& timer) {
	
	for (int i = 0; i < getMaxNumOfUserInputs(); i++) {
		if (_kbhit()) {
			char key = static_cast<char>(_getch());
			if (key == getEscCode()) { pauseGame(timer); }
			mario.keyPressed(key);
		}
		Sleep(getIntervalTime());
	}

}
