#include "Mario.h"

// Handle a key press event to update Mario's action
void Mario::keyPressed(char key) {
	key = static_cast<char>(std::tolower(key));
	setKey(key);

	if (isHoldingHammer()) {	
		// Hammer's direction calculates based on Mario's direction, so store it before updating Mario's direction
		Coord hammerPosBeforeMarioUpdate = getHammerPos();
	
		updateDirection();
	
		// Erase the hammer from the previous position. handle fast using of hammer while switching directions.
		if (isHoldingHammer()) {
			gotoxy(hammerPosBeforeMarioUpdate);
			pScreen->coloredCharPrint(pBoard->getChar(hammerPosBeforeMarioUpdate), pScreen->getColor("board"));
		}

		if (key == getHammerSymbol()) {
			setUseingHammer(true);
		}
	}

	else {
		updateDirection();
	}

	
}
// Update mario direction based on the key pressed
void Mario::updateDirection() {
	char key = getKey();
	for (size_t i = 0; i < numKeys; i++) {
		if (key == keys[i]) {
			lastDir = dir;
			dir = directions[i];
			return;
		}
	}
}
// Move Mario based on his current direction
void Mario::move() {  // was taken from lab excercise
	bool isFallSession = false;
	Coord currPos = getPos();
	Direction currDir = getDir();

	Coord oneBelow = { currPos.x, currPos.y + 1 };
	Coord twoBelow = { currPos.x, currPos.y + 2 };
	Coord oneAbove = { currPos.x, currPos.y - 1 };
	Coord twoAbove = { currPos.x, currPos.y - 2 };

	if (currDir == Direction::DOWN) {
		if (pBoard->isLadder(twoBelow) && pBoard->isFloor(oneBelow)) { // get down from a floor to a ladder
			setY(currPos.y + 2);
		} 
		else if (pBoard->isLadder(oneBelow)) { // get down of a ladder
			setY(currPos.y + 1);
		} 
	}
	else if (pBoard->isAir(oneBelow)) { // apply gravitation
		isFallSession = true;
		setY(currPos.y + 1);
	}
	
	else if (currDir == Direction::UP) {
		if (pBoard->isLadder(currPos)) {
			climbLadder();
		}
		else if ((pBoard->isAir(oneAbove) && pBoard->isAir(twoAbove)) || (pBoard->isAir(oneAbove) && pBoard->isHammer(twoAbove))) { // double jump or climp a ladder to upper floor
			handleMotionJump();
			setY(currPos.y - 2);
			setDir(Direction::STAY);
		}
	}

	else if (!pBoard->isValidCoord(currPos + currDir)) { // block the point if the next coordinate is invalid
		setDir(Direction::STAY);
	}

	else {
		setPos(currPos + currDir);		
	}
	
	isFallSession ? fallCounter++ : (fallCounter = 0);
}

//Make mario climb on the ladder
void Mario::climbLadder() {
	Coord currPos = getPos();
	Coord oneAbove = { currPos.x, currPos.y - 1};
	Coord twoAbove = { currPos.x, currPos.y - 2};

	if (pBoard->isFloor(oneAbove)) {
		if (pBoard->isAir(twoAbove)) { // climb a ladder to next floor.
			setDir({ 0, 0 }); // make mario stop moving up.
		}
		setY(currPos.y - 2); 
	}
	else { // climb a ladder 
		setY(currPos.y - 1);
	}
}

// Make mario jump two char up
void Mario::handleMotionJump() {
	Coord currPos = getPos();
	Direction _lastDir = getLastDir();

	if (pBoard->isValidCoord({ currPos.x + _lastDir.x, currPos.y - 1 })) {
		if (pBoard->isValidCoord({ currPos.x + 2 * _lastDir.x, currPos.y - 2 })) {
			setX(currPos.x + 2 * _lastDir.x); // motion jump
		}
	}
}

// erase Mario from the board
void Mario::erase() const {
	Entity::erase();
	if (isHoldingHammer()) {
		eraseHammer();
	}
}

// draw Mario on the board
void Mario::draw() const  {
	Entity::draw();
	if (isUsingHammer()) {
		drawHammer();
	}
}

// draw the hammer on the board
void Mario::drawHammer() const {
	Coord hammerPos = getHammerPos();
	if (pBoard->isValidCoord(hammerPos)) {
		gotoxy(hammerPos);
		pScreen->coloredCharPrint(getHammerSymbol(), getHammerColor());
	}
}

// Get the hammer direction
Direction Mario::getHammerDir() const {
	Direction currDir = getDir();

	// use the last direction if mario if not moving
	if (currDir == Direction::STAY || currDir == Direction::UP || currDir == Direction::DOWN) {
		Direction _lastDir = getLastDir();
		if (_lastDir == Direction::STAY || _lastDir == Direction::UP || _lastDir == Direction::DOWN) {
			currDir = getDefaultHammerDir();
		}
		else {
			currDir = getLastDir();
		}
	}
	return currDir;
}